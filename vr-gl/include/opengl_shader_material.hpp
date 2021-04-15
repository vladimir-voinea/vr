#pragma once

#include <shader_material.hpp>
#include "opengl_shader.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace vr::gl
{
	enum class uniform_type
	{
		mat4fv,
		vec3f,
		vec1i
	};

	struct uniform
	{
		std::string name;
		uniform_type type;
		struct
		{
			glm::mat4 mat4fv;
			glm::vec3 vec3f;
			int vec1i;
		} value;
	};

	class opengl_shader_material : public vr::shader_material
	{
	public:
		opengl_shader_material(const opengl_shader& shader, const std::vector<uniform>* uniforms);

		const opengl_shader& get_shader() const;
		const std::vector<uniform>* get_uniforms() const;

	private:
		const opengl_shader& m_shader;
		const std::vector<uniform>* m_uniforms;
	};
}