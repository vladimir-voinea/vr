#include "opengl_shader.hpp"

namespace vr::gl
{
	opengl_shader::opengl_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
		: m_vshader(vertex_shader_source)
		, m_fshader(fragment_shader_source)
	{}

	const std::string& opengl_shader::get_vertex_shader_source() const
	{
		return m_vshader;
	}

	const std::string& opengl_shader::get_fragment_shader_source() const
	{
		return m_fshader;
	}
}