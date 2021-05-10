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

		std::list<vr::gl::opengl_shader> shaders;
		std::list<std::vector<vr::gl::uniform>> uniforms;
		std::vector<std::unique_ptr<vr::gl::opengl_shader_material>> materials;

		std::list<vr::texture> textures;

		std::vector<vr::mesh> meshes;
	};

	//TODO work with either asset or path
	std::pair<std::unique_ptr<object3d>, model_data> load_model(const std::string& path);
}