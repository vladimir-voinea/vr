#pragma once

#include "opengl_shader_material.hpp"

#include <glm/glm.hpp>

namespace vr::gl
{
	class color_material : public opengl_shader_material
	{
	public:
		color_material(const glm::vec3& color);

		const glm::vec3& get_color() const;
		void set_color(const glm::vec3& color);

	private:
		vr::gl::opengl_shader m_shader;
		std::vector<uniform> m_uniforms;
		glm::vec3 m_color;
	};
}