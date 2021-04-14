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

std::string load_vertex_shader_code(const std::string& vertex_shader_name)
{
	const std::string vertex_path = "data/shaders/" + vertex_shader_name + ".vert";
	return get_vertex_shader_source(vertex_path);
}

std::string load_fragment_shader_code(const std::string& fragment_shader_name)
{
	const std::string fragment_path = "data/shaders/" + fragment_shader_name + ".frag";
	return get_fragment_shader_source(fragment_path);
}

shaders load_shaders(const std::string& vertex_shader_name, const std::string& fragment_shader_name)
{
	shaders result;

	result.vertex = vr::gl::shader(vr::gl::shader::type::vertex, load_vertex_shader_code(vertex_shader_name));
	result.fragment = vr::gl::shader(vr::gl::shader::type::fragment, load_fragment_shader_code(fragment_shader_name));
	result.program = vr::gl::shader_program(result.vertex, result.fragment);

	return result;
}
