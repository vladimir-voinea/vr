#include "model_loading.hpp"

#include <platform_manager_factory.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iterator>
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

			uniform mat4 vr_mvp;
			uniform mat4 vr_projection;
			uniform mat4 vr_view;
			uniform mat4 vr_model;
			uniform mat4 vr_modelview;
			uniform mat4 vr_normal;

			uniform vec3 vr_ambient;
			uniform vec3 vr_diffuse;
			uniform vec3 vr_specular;
			uniform float vr_shininess;

			void main(){

				gl_Position =  vr_mvp * vec4(vr_vertex_position, 1);
				uv = vr_vertex_uv;
			}
	)";

	constexpr auto fshader = R"(
			#version 300 es

			in highp vec2 uv;
			in highp vec3 normal;
			in highp vec3 position;

			out highp vec3 color;
			uniform sampler2D vr_texture_sampler;

			void main()
			{
				color = texture(vr_texture_sampler, uv).rgb;
			}
	)";

	void load_node(model& model, const aiScene* scene, const aiNode* assimp_node, object3d* parent, const aiMatrix4x4& accumulated_transformation)
	{
		object3d* current_node = nullptr;
		if (!parent)
		{
			model.root_node = std::make_unique<object3d>();
			current_node = model.root_node.get();
		}
		else
		{
			auto node = std::make_unique<object3d>();
			current_node = node.get();
			parent->add_child(std::move(node));
		}

		const auto assimp_transformation = assimp_node->mTransformation * accumulated_transformation;

		aiVector3D scale;
		aiVector3D rotation_axis;
		float rotation_angle;
		aiVector3D translate;
		assimp_transformation.Decompose(scale, rotation_axis, rotation_angle, translate);

		for (auto i = 0u; i < assimp_node->mNumMeshes; ++i)
		{
			const auto mesh_idx = assimp_node->mMeshes[i];
			vr::mesh* mesh = &model.data.meshes.at(mesh_idx);
			current_node->add_mesh(mesh);

			current_node->translate({ translate.x, translate.y, translate.z });
			current_node->rotate({ rotation_axis.x, rotation_axis.y, rotation_axis.z }, rotation_angle);
			current_node->scale({ scale.x, scale.y, scale.z });
		}

		for (auto i = 0u; i < assimp_node->mNumChildren; ++i)
		{
			load_node(model, scene, assimp_node->mChildren[i], current_node, assimp_transformation);
		}
	}

	void load_mesh(model& model, const aiMesh* mesh)
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

		vr::geometry* geometry = &model.data.geometries.emplace_back(std::move(loaded_geometry));
		vr::shader_material* material = model.data.materials.at(mesh->mMaterialIndex).get();
		vr::texture* texture = &model.data.textures.emplace_back("models/uvmap.png");

		model.data.meshes.emplace_back(geometry, material, texture);
	}

	void load_material(model& model, const aiMaterial* assimp_material)
	{

		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		float shininess;

		assimp_material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		assimp_material->Get(AI_MATKEY_SHININESS, shininess);

		std::vector<vr::gl::uniform> created_uniforms;
		vr::gl::uniform ambient_uniform;
		ambient_uniform.name = "vr_ambient";
		ambient_uniform.type = vr::gl::uniform_type::vec3f;
		ambient_uniform.value.vec3f = { ambient.r, ambient.g, ambient.b };
		created_uniforms.push_back(ambient_uniform);

		vr::gl::uniform diffuse_uniform;
		diffuse_uniform.name = "vr_diffuse";
		diffuse_uniform.type = vr::gl::uniform_type::vec3f;
		diffuse_uniform.value.vec3f = { diffuse.r, diffuse.g, diffuse.b };
		created_uniforms.push_back(diffuse_uniform);

		vr::gl::uniform specular_uniform;
		specular_uniform.name = "vr_specular";
		specular_uniform.type = vr::gl::uniform_type::vec3f;
		specular_uniform.value.vec3f = { specular.r, specular.g, specular.b };
		created_uniforms.push_back(specular_uniform);

		vr::gl::uniform shininess_uniform;
		shininess_uniform.name = "vr_shininess";
		shininess_uniform.type = vr::gl::uniform_type::vec1f;
		shininess_uniform.value.vec1f = shininess;
		created_uniforms.push_back(shininess_uniform);

		const auto uniforms = &model.data.uniforms.emplace_back(std::move(created_uniforms));
		const auto& shader = model.data.shaders.emplace_back(vshader, fshader);
		auto material = std::make_unique<vr::gl::opengl_shader_material>(shader, uniforms);

		model.data.materials.push_back(std::move(material));
	}

	model load_model(const std::string& asset_name)
	{
		const auto name = "models/" + asset_name + ".dae";

		auto am = vr::platform::get_platform_manager()->get_asset_manager();
		auto asset = am->get_asset_by_name(name);
		const auto file = am->read_file(asset);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(file.data(), file.size(), aiPostProcessSteps::aiProcess_ValidateDataStructure |
			aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
			aiPostProcessSteps::aiProcess_Triangulate);
		if (scene)
		{
			model result;

			const auto n_materials = scene->mNumMaterials;
			for (auto i = 0u; i < n_materials; ++i)
			{
				load_material(result, scene->mMaterials[i]);
			}

			const auto n_meshes = scene->mNumMeshes;
			for (auto i = 0u; i < n_meshes; ++i)
			{
				load_mesh(result, scene->mMeshes[i]);
			}

			const aiMatrix4x4 identity;
			load_node(result, scene, scene->mRootNode, nullptr, identity);

			return result;
		}
		else
		{
			const std::string message = "Could not import model: " + std::string(importer.GetErrorString());
			throw std::runtime_error(message);
		}
	}
}