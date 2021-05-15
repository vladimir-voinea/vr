#include "uniform.hpp"

namespace vr
{
	uniform make_uniform_base(const std::string& name)
	{
		uniform u;
		u.name = name;

		return u;
	}

	uniform make_uniform(const std::string& name, const glm::mat4& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::mat4fv;
		b.value.mat4fv = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const glm::mat3& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::mat3fv;
		b.value.mat3fv = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const glm::vec3& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::vec3f;
		b.value.vec3f = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const glm::vec4& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::vec4f;
		b.value.vec4f = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const int& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::vec1i;
		b.value.vec1i = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const float& value)
	{
		auto b = make_uniform_base(name);
		b.type = uniform_type::vec1f;
		b.value.vec1f = value;

		return b;
	}

	uniform make_uniform(const std::string& name, const bool& value)
	{
		const auto int_value = static_cast<int>(value);
		return make_uniform(name, int_value);
	}
}