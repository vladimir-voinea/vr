#include "opengl_shader.hpp"

#include "vr-opengl.h"

namespace vr::gl
{
	/*
			opengl_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::vector<uniform>& uniforms);

		const std::string& get_vertex_shader_source() const;
		const std::string& get_fragment_shader_source() const;
		const std::vector<uniform>& get_uniforms() const;
	*/

	opengl_shader::opengl_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::vector<uniform>& uniforms)
		: m_vertex_shader_source(vertex_shader_source)
		, m_fragment_shader_source(fragment_shader_source)
		, m_uniforms(uniforms)
	{}

	const std::string& opengl_shader::get_vertex_shader_source() const
	{
		return m_vertex_shader_source;
	}
	
	const std::string& opengl_shader::get_fragment_shader_source() const
	{
		return m_fragment_shader_source;
	}
	
	const std::vector<uniform>& opengl_shader::get_uniforms() const
	{
		return m_uniforms;
	}
}