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
		vec4f,
		vec1i,
		vec1f
	};

	struct uniform
	{
		std::string name;
		uniform_type type;
		struct
		{
			glm::mat4 mat4fv;
			glm::vec3 vec3f;
			glm::vec4 vec4f;
			int vec1i;
			float vec1f;
		} value;
	};

	class opengl_shader_material : public vr::shader_material
	{
	public:
		opengl_shader_material(const opengl_shader& shader, const std::vector<uniform>* uniforms);
		~opengl_shader_material() override;

		const opengl_shader& get_shader() const;
		const std::vector<uniform>* get_uniforms() const;

	private:
		const opengl_shader& m_shader;
		const std::vector<uniform>* m_uniforms;
	};
}