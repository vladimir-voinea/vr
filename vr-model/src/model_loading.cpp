#include "model_loading.hpp"

#include <platform_manager_factory.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <spdlog/spdlog.h>

#include <iterator>
#include <filesystem>
#include <stdexcept>

namespace vr::model
{
	constexpr auto vshader = R"(
			#version 300 es

			in vec3 vr_vertex_position;
			in vec3 vr_vertex_normal;
			in vec4 vr_vertex_color;
			in vec2 vr_vertex_uv;

			out vec2 uv;
			out vec3 normal;
			out vec3 position;

			uniform vec3 vr_view_position;
			uniform mat4 vr_mvp;
			uniform mat4 vr_projection;
			uniform mat4 vr_view;
			uniform mat4 vr_model;
			uniform mat4 vr_modelview;
			uniform mat4 vr_normal;

			void main(){
				vec4 position4 = vr_mvp * vec4(vr_vertex_position, 1);
				position = vec3(position4.xyz);
				gl_Position = position4;

				vec4 normal4 = vr_normal * vec4(vr_vertex_normal, 0.f);
				normal = vec3(normal4.xyz);
				
				uv = vr_vertex_uv;
			}
	)";

	constexpr auto fshader = R"(
			#version 300 es

			struct vr_material_t
			{
				highp vec3 ambient;
				highp vec3 diffuse;
				highp vec3 specular;
				highp float shininess;
			};

			struct vr_light_t
			{
				highp vec3 position;
				highp vec3 ambient;
				highp vec3 diffuse;
				highp vec3 specular;
			};

			in highp vec2 uv;
			in highp vec3 normal;
			in highp vec3 position;
			out highp vec4 out_color4;

			uniform highp vec3 vr_view_position;
			uniform vr_material_t vr_material;
			uniform vr_light_t vr_light;
			uniform sampler2D vr_texture_sampler;

			void main()
			{
				highp vec3 ambient = vr_light.ambient * vr_material.ambient;
				
				highp vec3 normalized_normal = normalize(normal);
				highp vec3 light_direction = normalize(vr_light.position - position);
				highp float diff = max(dot(normalized_normal, light_direction), 0.f);
				highp vec3 diffuse = vr_light.diffuse * (diff * vr_material.diffuse);

				highp vec3 view_direction = normalize(vr_view_position - position);
				highp vec3 reflect_direction = reflect(-light_direction, normalized_normal);
				highp float spec = pow(max(dot(view_direction, reflect_direction), 0.f), vr_material.shininess);
				highp vec3 specular = vr_light.specular * (spec * vr_material.specular);

				highp vec3 result = ambient + diffuse + specular;
				out_color4 = vec4(result, 1.f);
			}
	)";

	void load_node(model_data& data, std::unique_ptr<object3d>& node, const aiScene* scene, const aiNode* assimp_node, object3d* parent, const aiMatrix4x4& accumulated_transformation)
	{
		object3d* current_node = nullptr;
		if (!parent)
		{
			node = std::make_unique<object3d>();
			current_node = node.get();
		}
		else
		{
			auto new_node = std::make_unique<object3d>();
			current_node = new_node.get();
			parent->add_child(std::move(new_node));
		}

		const auto assimp_transformation = assimp_node->mTransformation;

		aiVector3D scale;
		aiVector3D rotation_axis;
		float rotation_angle;
		aiVector3D translate;
		assimp_transformation.Decompose(scale, rotation_axis, rotation_angle, translate);

		current_node->translate({ translate.x, translate.y, translate.z });
		current_node->rotate({ rotation_axis.x, rotation_axis.y, rotation_axis.z }, rotation_angle);
		current_node->scale({ scale.x, scale.y, scale.z });

		for (auto i = 0u; i < assimp_node->mNumMeshes; ++i)
		{
			const auto mesh_idx = assimp_node->mMeshes[i];
			vr::mesh* mesh = &data.meshes.at(mesh_idx);
			current_node->add_mesh(mesh);
		}

		for (auto i = 0u; i < assimp_node->mNumChildren; ++i)
		{
			load_node(data, node, scene, assimp_node->mChildren[i], current_node, assimp_transformation);
		}
	}

	void load_mesh(model_data& data, const aiMesh* mesh)
	{
		vr::geometry loaded_geometry;

		loaded_geometry.attributes["vr_vertex_position"].components = 3;
		loaded_geometry.attributes["vr_vertex_position"].type = vr::attribute::data_type::t_float;
		{
			const auto begin = reinterpret_cast<const uint8_t*>(mesh->mVertices);
			const auto end = reinterpret_cast<const uint8_t*>(&mesh->mVertices[mesh->mNumVertices]);
			std::copy(begin, end, std::back_inserter(loaded_geometry.attributes["vr_vertex_position"].data));
		}

		for (auto i = 0u; i < mesh->mNumFaces; ++i)
		{
			loaded_geometry.indices.push_back(mesh->mFaces[i].mIndices[0]);
			loaded_geometry.indices.push_back(mesh->mFaces[i].mIndices[1]);
			loaded_geometry.indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		if (mesh->HasNormals())
		{
			loaded_geometry.attributes["vr_vertex_normal"].components = 3;
			loaded_geometry.attributes["vr_vertex_normal"].type = vr::attribute::data_type::t_float;
			const auto begin = reinterpret_cast<const uint8_t*>(mesh->mNormals);
			std::copy(begin, begin + mesh->mNumVertices * sizeof(decltype(*mesh->mNormals)), std::back_inserter(loaded_geometry.attributes["vr_vertex_normal"].data));
		}

		if (mesh->HasVertexColors(0))
		{
			loaded_geometry.attributes["vr_vertex_color"].components = 4;
			loaded_geometry.attributes["vr_vertex_color"].type = vr::attribute::data_type::t_float;
			const auto begin = reinterpret_cast<const uint8_t*>(mesh->mColors);
			std::copy(begin, begin + mesh->mNumVertices + sizeof(decltype(*mesh->mColors)), std::back_inserter(loaded_geometry.attributes["vr_vertex_color"].data));
		}

		if (mesh->HasTextureCoords(0))
		{
			loaded_geometry.attributes["vr_vertex_uv"].type = vr::attribute::data_type::t_float;
			loaded_geometry.attributes["vr_vertex_uv"].components = 2;
			for (auto i = 0u; i < mesh->mNumVertices; ++i)
			{
				const auto uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				const auto begin = reinterpret_cast<const uint8_t*>(&uv);
				std::copy(begin, begin + sizeof(uv), std::back_inserter(loaded_geometry.attributes["vr_vertex_uv"].data));
			}
		}

		vr::geometry* geometry = &data.geometries.emplace_back(std::move(loaded_geometry));
		vr::shader_material* material = data.materials.at(mesh->mMaterialIndex).get();
		vr::texture* texture = &data.textures.emplace_back("models/uvmap.png");

		data.meshes.emplace_back(geometry, material);
	}

	void load_texture(model_data& data, const aiTexture* assimp_texture)
	{
		std::vector<vr::texel> texels;

		const auto width = assimp_texture->mWidth;
		const auto height = assimp_texture->mHeight;
		const auto n_texels = width * height;
		
		for (auto i = 0u; i < n_texels; ++i)
		{
			const aiTexel& assimp_texel = assimp_texture->pcData[i];
			texels.emplace_back(assimp_texel.r, assimp_texel.g, assimp_texel.b);
		}

		const std::string path = assimp_texture->mFilename.C_Str() ? assimp_texture->mFilename.C_Str() : "";

		data.textures.emplace_back(path, std::move(texels), width, height);
		
	}

	void load_material(model_data& data, const aiMaterial* assimp_material)
	{
		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		float shininess;

		assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		assimp_material->Get(AI_MATKEY_SHININESS, shininess);

		std::vector<vr::gl::uniform> uniforms;
		vr::gl::uniform ambient_uniform;
		ambient_uniform.name = "vr_material.ambient";
		ambient_uniform.type = vr::gl::uniform_type::vec3f;
		ambient_uniform.value.vec3f = { ambient.r, ambient.g, ambient.b };
		uniforms.push_back(ambient_uniform);

		vr::gl::uniform diffuse_uniform;
		diffuse_uniform.name = "vr_material.diffuse";
		diffuse_uniform.type = vr::gl::uniform_type::vec3f;
		diffuse_uniform.value.vec3f = { diffuse.r, diffuse.g, diffuse.b };
		uniforms.push_back(diffuse_uniform);

		vr::gl::uniform specular_uniform;
		specular_uniform.name = "vr_material.specular";
		specular_uniform.type = vr::gl::uniform_type::vec3f;
		specular_uniform.value.vec3f = { specular.r, specular.g, specular.b };
		uniforms.push_back(specular_uniform);

		vr::gl::uniform shininess_uniform;
		shininess_uniform.name = "vr_material.shininess";
		shininess_uniform.type = vr::gl::uniform_type::vec1f;
		shininess_uniform.value.vec1f = shininess;
		uniforms.push_back(shininess_uniform);

		auto material = std::make_unique<vr::gl::opengl_shader_material>(*data.shader, uniforms);

		/*assimp_material->Get(AI_MATKEY_TEXTURE_AMBIENT,)*/

		data.materials.push_back(std::move(material));
	}

	std::pair<std::unique_ptr<object3d>, model_data> load_model(const std::string& path)
	{
#ifdef WIN32
		const auto name = path;
		if (!std::filesystem::exists(path))
		{
			throw std::runtime_error("Could not find " + path);
		}
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(name,
#else
		const auto name = "models/" + asset_name;
		auto am = vr::platform::get_platform_manager()->get_asset_manager();
		auto asset = am->get_asset_by_name(name);
		const auto file = am->read_file(asset);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(file.data(), file.size(),
#endif
			aiPostProcessSteps::aiProcess_ValidateDataStructure
			| aiPostProcessSteps::aiProcess_JoinIdenticalVertices 
			| aiPostProcessSteps::aiProcess_Triangulate
			| aiPostProcessSteps::aiProcess_EmbedTextures
		);
		if (scene)
		{
			std::pair<std::unique_ptr<object3d>, model_data> result;
			result.second.shader = std::make_unique<vr::gl::opengl_shader>(vshader, fshader);

			const auto n_textures = scene->mNumTextures;
			for (auto i = 0u; i < n_textures; ++i)
			{
				load_texture(result.second, scene->mTextures[i]);
			}

			const auto n_materials = scene->mNumMaterials;
			for (auto i = 0u; i < n_materials; ++i)
			{
				load_material(result.second, scene->mMaterials[i]);
			}

			const auto n_meshes = scene->mNumMeshes;
			for (auto i = 0u; i < n_meshes; ++i)
			{
				load_mesh(result.second, scene->mMeshes[i]);
			}

			const aiMatrix4x4 identity;
			load_node(result.second, result.first, scene, scene->mRootNode, nullptr, identity);

			return result;
		}
		else
		{
			const std::string message = "Could not import model: " + std::string(importer.GetErrorString());
			throw std::runtime_error(message);
		}
	}
}