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

std::string get_vertex_shader_source(const std::string& path)
{
	return read_file(path);
}

std::string get_fragment_shader_source(const std::string& path)
{
	return read_file(path);
}

shaders load_shaders(const std::string& vertex_shader_name, const std::string& fragment_shader_name)
{
	const std::string vertex_path = "data/shaders/" + vertex_shader_name + ".vert";
	const std::string fragment_path = "data/shaders/" + fragment_shader_name + ".frag";

	shaders result;

	result.vertex = vr::gl::shader(vr::gl::shader::type::vertex, get_vertex_shader_source(vertex_path));
	result.fragment = vr::gl::shader(vr::gl::shader::type::fragment, get_fragment_shader_source(fragment_path));
	result.program = vr::gl::shader_program(result.vertex, result.fragment);

	return result;
}