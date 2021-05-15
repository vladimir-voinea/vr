#pragma once

#include <glm/glm.hpp>

#include <string>

namespace vr
{
	enum class uniform_type
	{
		mat4fv,
		mat3fv,
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
			glm::mat3 mat3fv;
			glm::vec3 vec3f;
			glm::vec4 vec4f;
			int vec1i;
			float vec1f;
		} value;
	};

	uniform make_uniform(const std::string& name, const glm::mat4& value);
	uniform make_uniform(const std::string& name, const glm::mat3& value);
	uniform make_uniform(const std::string& name, const glm::vec3& value);
	uniform make_uniform(const std::string& name, const glm::vec4& value);
	uniform make_uniform(const std::string& name, const int& value);
	uniform make_uniform(const std::string& name, const float& value);
	uniform make_uniform(const std::string& name, const bool& value);
}