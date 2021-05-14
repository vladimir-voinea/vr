#pragma once

#include <shader_material.hpp>
#include "opengl_shader.hpp"
#include <uniform.hpp>
#include <texture.hpp>

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace vr::gl
{
	class opengl_shader_material : public vr::shader_material
	{
	public:
		opengl_shader_material(const opengl_shader& shader, std::vector<uniform> uniforms, std::vector<const texture*> textures);
		opengl_shader_material(const opengl_shader& shader, std::vector<uniform> uniforms);
		opengl_shader_material(const opengl_shader& shader);
		virtual ~opengl_shader_material() override;

		const opengl_shader& get_shader() const;
		const std::vector<uniform>& get_uniforms() const;
		std::vector<uniform>& get_uniforms();

		const std::vector<const texture*>& get_textures() const;

	private:
		const opengl_shader& m_shader;
		std::vector<uniform> m_uniforms;
		std::vector<const texture*> m_textures;
	};
}