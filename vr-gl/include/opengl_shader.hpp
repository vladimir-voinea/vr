#pragma once

#include <shader_material.hpp>

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <unordered_map>
#include <variant>

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

	class opengl_shader : public vr::shader_material
	{
	public:
		opengl_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source, const std::vector<uniform>& uniforms);

		const std::string& get_vertex_shader_source() const;
		const std::string& get_fragment_shader_source() const;
		const std::vector<uniform>& get_uniforms() const;
	};
}