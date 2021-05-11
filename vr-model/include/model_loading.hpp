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
		std::unique_ptr<vr::gl::opengl_shader> shader;

		std::list<vr::geometry> geometries;
		std::list<vr::texture> textures;
		std::vector<std::unique_ptr<vr::gl::opengl_shader_material>> materials;

		std::vector<vr::mesh> meshes;
	};

	//TODO work with either asset or path
	std::pair<std::unique_ptr<object3d>, model_data> load_model(const std::string& path);
}