#pragma once

#include "vr-opengl.h"
#include "vr-gl.hpp"

#include <renderer_object.hpp>

#include <map>
#include <vector>


namespace vr::gl
{
	// texture instance
	struct loaded_texture
	{
		GLuint id;
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
		GLuint id;
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