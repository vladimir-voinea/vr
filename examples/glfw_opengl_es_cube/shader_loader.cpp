#include "shader_loader.hpp"

#include <string>

std::string get_vertex_shader_source()
{
	std::string result = 

	#include "shaders/vertex_shader.vert"
		;

	return result;
}

std::string get_fragment_shader_source()
{
	std::string result =

	#include "shaders/fragment_shader.frag"
		;

	return result;
}

shaders load_shaders()
{
	shaders result;

	result.vertex = vr::gl::shader(vr::gl::shader::type::vertex, get_vertex_shader_source());
	result.fragment = vr::gl::shader(vr::gl::shader::type::fragment, get_fragment_shader_source());
	result.program = vr::gl::shader_program(result.vertex, result.fragment);

	return result;
}