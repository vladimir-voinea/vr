#include "opengl_shader.hpp"

#include "vr-opengl.h"

namespace vr::gl
{
	opengl_shader_material::opengl_shader_material(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::vector<uniform>& uniforms)
		: m_vertex_shader_source(vertex_shader_source)
		, m_fragment_shader_source(fragment_shader_source)
		, m_uniforms(uniforms)
	{}

	const std::string& opengl_shader_material::get_vertex_shader_source() const
	{
		return m_vertex_shader_source;
	}
	
	const std::string& opengl_shader_material::get_fragment_shader_source() const
	{
		return m_fragment_shader_source;
	}
	
	const std::vector<uniform>& opengl_shader_material::get_uniforms() const
	{
		return m_uniforms;
	}
}