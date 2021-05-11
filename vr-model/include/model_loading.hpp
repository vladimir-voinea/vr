#pragma once

#include <vr.hpp>
#include <vr-gl.hpp>

#include <list>
#include <vector>
#include <string>
#include <utility>
#include <memory>

namespace vr::model
{
	struct model_data
	{
		std::list<vr::geometry> geometries;
		std::unique_ptr<vr::gl::opengl_shader> shader;
		std::vector<std::unique_ptr<vr::gl::opengl_shader_material>> materials;

		std::vector<vr::texture> textures;
		std::list<std::vector<const texture*>> textures_per_material;

		std::vector<vr::mesh> meshes;
	};

	//TODO work with either asset or path
	std::pair<std::unique_ptr<object3d>, model_data> load_model(const std::string& path);
}