#pragma once

#include "vr-opengl.h"
#include "vr-gl.hpp"

#include <geometry.hpp>

#include <map>
#include <vector>


namespace vr::gl
{
	// texture instance
	struct loaded_texture
	{
		GLuint id;
		GLenum target;
	};
	
	// shader instance
	struct loaded_shader 
	{
		shader vertex;
		shader fragment;
		shader_program program;
	};

	struct vertex_buffer 
	{
		struct attribute_layout
		{
			uint64_t start;
			uint64_t size;
			uint8_t components;
			vr::attribute::data_type type;
		};

		GLuint id;
		std::unordered_map<std::string, attribute_layout> loaded_attributes;
	};

	// geometry instance
	struct loaded_vertex_array_object
	{
		GLuint id;
		vertex_buffer buffer;
		vertex_buffer indices;
		size_t indices_size;
	};

	struct loaded_geometry
	{
		loaded_vertex_array_object vao;
	};
}