#include "opengl_shader_material.hpp"
#include "opengl_shader.hpp"
#include "vr-opengl.h"

namespace vr::gl
{
	opengl_shader_material::opengl_shader_material(const opengl_shader& shader, const std::vector<uniform>& uniforms)
		: m_shader(shader)
		, m_uniforms(uniforms)
	{}

	const opengl_shader& opengl_shader_material::get_shader() const
	{
		return m_shader;
	}
	
	const std::vector<uniform>& opengl_shader_material::get_uniforms() const
	{
		return m_uniforms;
	}
}