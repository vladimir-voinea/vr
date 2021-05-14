#include "model_loading.hpp"
#include "shader_repository.hpp"

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

		if (mesh->HasTangentsAndBitangents())
		{
			loaded_geometry.attributes["vr_vertex_tangent"].components = 3;
			loaded_geometry.attributes["vr_vertex_tangent"].type = vr::attribute::data_type::t_float;
			const auto t_begin = reinterpret_cast<const uint8_t*>(mesh->mTangents);
			std::copy(t_begin, t_begin + mesh->mNumVertices * sizeof(decltype(*mesh->mTangents)), std::back_inserter(loaded_geometry.attributes["vr_vertex_tangent"].data));

			loaded_geometry.attributes["vr_vertex_bitangent"].components = 3;
			loaded_geometry.attributes["vr_vertex_bitangent"].type = vr::attribute::data_type::t_float;
			const auto b_begin = reinterpret_cast<const uint8_t*>(mesh->mBitangents);
			std::copy(b_begin, b_begin + mesh->mNumVertices * sizeof(decltype(*mesh->mBitangents)), std::back_inserter(loaded_geometry.attributes["vr_vertex_bitangent"].data));
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

		data.meshes.emplace_back(geometry, material);
	}

	vr::texture* load_texture(model_data& data, const aiTexture* assimp_texture)
	{
		std::vector<vr::texel> texels;

		const auto width = assimp_texture->mWidth;
		const auto height = assimp_texture->mHeight;
		const auto n_texels = width * height;

		if (height)
		{
			for (auto i = 0u; i < n_texels; ++i)
			{
				const aiTexel& assimp_texel = assimp_texture->pcData[i];
				texels.emplace_back(assimp_texel.r, assimp_texel.g, assimp_texel.b);
			}

			return &data.textures.emplace_back(std::move(texels), width, height);
		}
		else if(width)
		{
			const auto start = reinterpret_cast<uint8_t*>(assimp_texture->pcData);
			const auto end = start + width;
			auto compressed_data = std::vector<uint8_t>{ start, end };
			return &data.textures.emplace_back(std::move(compressed_data));
		}
		else
		{
			return nullptr;
		}
	}

	void load_material(model_data& data, const aiMaterial* assimp_material, const aiScene* scene)
	{
		std::vector<vr::gl::uniform> uniforms;

		auto add_color_if_found = [&uniforms, &assimp_material](const std::string& uniform_name, const std::string& color_check_uniform_name, auto&& ...key)
		{
			int have_color = 0;
			aiColor3D color;
			if (assimp_material->Get(key..., color) == aiReturn_SUCCESS)
			{
				have_color = 1;
				vr::gl::uniform color_uniform;
				color_uniform.name = uniform_name;
				color_uniform.type = vr::gl::uniform_type::vec3f;
				color_uniform.value.vec3f = glm::vec3{ color.r, color.g, color.b };
				uniforms.push_back(color_uniform);
			}
			else
			{
				spdlog::info("No color");
			}

			vr::gl::uniform have_color_uniform;
			have_color_uniform.name = color_check_uniform_name;
			have_color_uniform.type = vr::gl::uniform_type::vec1i;
			have_color_uniform.value.vec1i = have_color;
			uniforms.push_back(have_color_uniform);
		};

		auto add_float_if_found = [&uniforms, &assimp_material](const std::string& uniform_name, const std::string& item_check_uniform_name, auto&& ...key)
		{
			int have_item = 0;
			float item;
			if (assimp_material->Get(key..., item) == aiReturn_SUCCESS)
			{
				have_item = 1;
				vr::gl::uniform item_uniform;
				item_uniform.name = uniform_name;
				item_uniform.type = vr::gl::uniform_type::vec3f;
				item_uniform.value.vec1f = item;
				uniforms.push_back(item_uniform);
			}

			vr::gl::uniform have_item_uniform;
			have_item_uniform.name = item_check_uniform_name;
			have_item_uniform.type = vr::gl::uniform_type::vec1i;
			have_item_uniform.value.vec1i = have_item;
			uniforms.push_back(have_item_uniform);
		};

		auto get_texture = [&assimp_material, &scene, &data](aiTextureType type, unsigned int index) -> vr::texture*
		{
			aiString path;
			aiTextureMapping mapping;
			unsigned int uv_index = 0;
			ai_real blend;
			aiTextureOp operation;
			aiTextureMapMode map_mode;
			if (assimp_material->GetTexture(type, index, &path, &mapping, &uv_index, &blend, &operation, &map_mode) == aiReturn_SUCCESS)
			{
				assert(mapping == aiTextureMapping_UV);
				assert(uv_index == 0);
				if (auto texture = scene->GetEmbeddedTexture(path.C_Str()); texture)
				{
					auto* loaded_texture = load_texture(data, texture);
					assert(loaded_texture != nullptr);
					
					return loaded_texture;
				}
				else
				{
					spdlog::info("Texture not embedded. Path: {}", path.C_Str());
					auto am = vr::platform::get_platform_manager()->get_asset_manager();
					if (am->exists({ path.C_Str() }))
					{
						return &data.textures.emplace_back(vr::texture(std::string(path.C_Str())));
					}
					else
					{
						spdlog::error("Texture file {} does not exist", path.C_Str());
						return nullptr;
					}
				}
			}
			else
			{
				return nullptr;
			}
		};

		std::vector<const vr::texture*> textures;
		auto add_if_found = [&get_texture, &textures, &uniforms](aiTextureType type, unsigned int index, 
			const std::string& uniform_name, const std::string& sampler_check_uniform_name)
		{
			int have_texture = 0;
			if (const auto texture = get_texture(type, index); texture)
			{
				have_texture = 1;
				textures.push_back(texture);

				vr::gl::uniform texture_sampler_uniform;
				texture_sampler_uniform.name = uniform_name;
				texture_sampler_uniform.type = vr::gl::uniform_type::vec1i;
				texture_sampler_uniform.value.vec1i = textures.size() - 1;
				uniforms.push_back(texture_sampler_uniform);
			}

			vr::gl::uniform have_texture_sampler_uniform;
			have_texture_sampler_uniform.name = sampler_check_uniform_name;
			have_texture_sampler_uniform.type = vr::gl::uniform_type::vec1i;
			have_texture_sampler_uniform.value.vec1i = have_texture;
			uniforms.push_back(have_texture_sampler_uniform);
		};

		add_color_if_found("vr_material.ambient_color", "vr_material.have_ambient_color", AI_MATKEY_COLOR_AMBIENT);
		add_color_if_found("vr_material.diffuse_color", "vr_material.have_diffuse_color", AI_MATKEY_COLOR_DIFFUSE);
		add_color_if_found("vr_material.specular_color", "vr_material.have_specular_color", AI_MATKEY_COLOR_SPECULAR);
		add_float_if_found("vr_material.shininess", "vr_material.have_shininess", AI_MATKEY_SHININESS);

		add_if_found(aiTextureType_AMBIENT, 0, "vr_material.ambient_texture", "vr_material.have_ambient_texture");
		add_if_found(aiTextureType_DIFFUSE, 0, "vr_material.diffuse_texture", "vr_material.have_diffuse_texture");
		add_if_found(aiTextureType_SPECULAR, 0, "vr_material.specular_texture", "vr_material.have_specular_texture");
		add_if_found(aiTextureType_NORMALS, 0, "vr_material.normal_texture", "vr_material.have_normal_texture");

		auto material = std::make_unique<vr::gl::opengl_shader_material>(*data.shader, uniforms, textures);

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
			| aiPostProcessSteps::aiProcess_GenSmoothNormals
			| aiPostProcessSteps::aiProcess_CalcTangentSpace
			| aiPostProcessSteps::aiProcess_FlipUVs
		);
		if (scene)
		{
			std::pair<std::unique_ptr<object3d>, model_data> result;
			result.second.shader = std::make_unique<vr::gl::opengl_shader>(vr::model::get_phong_vshader(), vr::model::get_phong_fshader());

			const auto n_materials = scene->mNumMaterials;
			for (auto i = 0u; i < n_materials; ++i)
			{
				load_material(result.second, scene->mMaterials[i], scene);
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