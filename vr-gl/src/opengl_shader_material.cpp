#include "opengl_shader_material.hpp"
#include "opengl_shader.hpp"
#include "vr-opengl.h"

namespace vr::gl
{
	opengl_shader_material::opengl_shader_material(const opengl_shader& shader, std::vector<uniform> uniforms, std::vector<const texture*> textures)
		: m_shader(shader)
		, m_uniforms(std::move(uniforms))
		, m_textures(textures)
	{}

	opengl_shader_material::opengl_shader_material(const opengl_shader& shader, std::vector<uniform> uniforms)
		: m_shader(shader)
		, m_uniforms(std::move(uniforms))
	{}

	opengl_shader_material::opengl_shader_material(const opengl_shader & shader)
		: m_shader(shader)
	{}

	opengl_shader_material::~opengl_shader_material() = default;

	const opengl_shader& opengl_shader_material::get_shader() const
	{
		return m_shader;
	}

	const std::vector<uniform>& opengl_shader_material::get_uniforms() const
	{
		return m_uniforms;
	}

	std::vector<uniform>& opengl_shader_material::get_uniforms()
	{
		return m_uniforms;
	}

	const std::vector<const texture*>& opengl_shader_material::get_textures() const
	{
		return m_textures;
	}
}