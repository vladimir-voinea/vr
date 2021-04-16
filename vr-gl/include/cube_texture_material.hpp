#pragma once

#include "opengl_shader_material.hpp"

#include <glm/glm.hpp>

namespace vr::gl
{
	class cube_texture_material : public opengl_shader_material
	{
	public:
		cube_texture_material();

	private:
		vr::gl::opengl_shader m_shader;
		std::vector<uniform> m_uniforms;
	};
}