#include "shader_loader.hpp"

#include <fstream>
#include <string>
#include <streambuf>

std::string read_file(const std::string& path)
{
	std::ifstream f(path);
	if (!f.good())
	{
		const std::string message = "File not found: " + path;
		throw std::runtime_error(message);
	}

	std::string result((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	return result;
}

std::string get_vertex_shader_source()
{
	return read_file("data/shaders/vertex_shader.vert");
}

std::string get_fragment_shader_source()
{
	return read_file("data/shaders/fragment_shader.frag");
}

shaders load_shaders()
{
	shaders result;

	result.vertex = vr::gl::shader(vr::gl::shader::type::vertex, get_vertex_shader_source());
	result.fragment = vr::gl::shader(vr::gl::shader::type::fragment, get_fragment_shader_source());
	result.program = vr::gl::shader_program(result.vertex, result.fragment);

	return result;
}