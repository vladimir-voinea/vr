#include "color_mateiral.hpp"

namespace
{
	const auto vshader = R"(
		#version 330 core

		layout(location = 0) in vec3 vr_vertex_position;
		layout(location = 1) in vec3 vr_vertex_normal;
		layout(location = 2) in vec4 vr_vertex_color;
		layout(location = 3) in vec2 vr_vertex_uv;

		uniform mat4 vr_mvp;
		uniform mat4 vr_projection;
		uniform mat4 vr_view;
		uniform mat4 vr_model;

		void main() {

			gl_Position = vr_mvp * vec4(vr_vertex_position, 1);
		}
		)";

	const auto fshader = R"(
		#version 330 core

		out vec3 color;
		uniform vec4 input_color;

		void main()
		{
			color = vec3(input_color.x, input_color.y, input_color.z);
		}
		)";
}

namespace vr::gl
{
	color_material::color_material(const glm::vec4& color)
		: opengl_shader_material(m_shader, m_uniforms)
		, m_shader(vshader, fshader)
		, m_color(color)
	{
		uniform color_uniform;
		color_uniform.name = "input_color";
		color_uniform.type = uniform_type::vec4f;
		color_uniform.value.vec4f = glm::normalize(m_color);
		m_uniforms.push_back(color_uniform);
	}

	const glm::vec4& color_material::get_color() const
	{
		return m_color;
	}

	void color_material::set_color(const glm::vec4& color)
	{
		m_color = color;
		m_uniforms.front().value.vec4f = glm::normalize(m_color);
	}
}