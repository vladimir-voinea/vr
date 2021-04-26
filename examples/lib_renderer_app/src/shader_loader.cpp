#include "shader_loader.hpp"

#include <platform_manager_factory.hpp>

#include <string>

std::string read_file(const std::string& name)
{
	auto am = vr::platform::get_platform_manager()->get_asset_manager();
	auto file_binary = am->read_file(am->get_asset_by_name(name));

	return std::string{ file_binary.begin(), file_binary.end() };
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
	const std::string vertex_path = "shaders/" + vertex_shader_name + ".vert";
	return get_vertex_shader_source(vertex_path);
}

std::string load_fragment_shader_code(const std::string& fragment_shader_name)
{
	const std::string fragment_path = "shaders/" + fragment_shader_name + ".frag";
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
