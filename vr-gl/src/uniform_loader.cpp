#include "uniform_loader.hpp"
#include "builtin_uniforms.hpp"
#include "vr-opengl.h"

#include <spdlog/spdlog.h>

#include <algorithm>

namespace vr::gl
{
	uniform_loader::uniform_loader(const shader_program& program)
		: m_program(program)
	{
	}

	uniform_loader::~uniform_loader() = default;

	void uniform_loader::load_uniform(const vr::uniform& uniform) const
	{
		const auto& uniforms = m_program.get_uniform_names();
		if (has_uniform(uniform.name))
		{
			spdlog::debug("Checking uniform location for uniform {}", uniform.name);
			const auto location = glGetUniformLocation(m_program.get_id(), uniform.name.c_str());
			spdlog::debug("Got location {}", location);

			std::string loaded_type = "NONE";
			using ut = vr::uniform_type;
			switch (uniform.type)
			{
			case ut::mat4fv:
			{
				loaded_type = "mat4fv";
				glUniformMatrix4fv(location, 1, GL_FALSE, &uniform.value.mat4fv[0][0]);
				break;
			}
			case ut::mat3fv:
			{
				loaded_type = "mat3fv";
				glUniformMatrix3fv(location, 1, GL_FALSE, &uniform.value.mat3fv[0][0]);
				break;
			}
			case ut::vec4f:
			{
				loaded_type = "4f";
				const glm::vec4& value = uniform.value.vec4f;
				glUniform4f(location, value.x, value.y, value.z, value.w);
				break;
			}
			case ut::vec3f:
			{
				loaded_type = "3f";
				const glm::vec3& value = uniform.value.vec3f;
				glUniform3f(location, value.x, value.y, value.z);
				break;
			}
			case ut::vec1i:
			{
				loaded_type = "1i";
				glUniform1i(location, uniform.value.vec1i);
				break;
			}
			case ut::vec1f:
			{
				loaded_type = "1f";
				glUniform1f(location, uniform.value.vec1f);
				break;
			}
			}
			spdlog::debug("Loaded a {} uniform", loaded_type);
		}
		else
		{
			if (uniform.name != builtin_view_position_uniform_name &&
				uniform.name != builtin_mvp_uniform_name && uniform.name != builtin_projection_uniform_name &&
				uniform.name != builtin_view_uniform_name && uniform.name != builtin_model_uniform_name &&
				uniform.name != builtin_normal_uniform_name && uniform.name != builtin_have_shininess_uniform_name)
			{
				spdlog::debug("Uniform {0} not found in program {1}", uniform.name, m_program.get_id());
			}
		}
	}

	bool uniform_loader::has_uniform(const std::string& name) const
	{
		return std::find(m_program.get_uniform_names().begin(), m_program.get_uniform_names().end(), name) != m_program.get_uniform_names().end();
	}
}