#pragma once

#include "opengl.h"

#include <string>

namespace vr::gl
{
	class shader
	{
	public:
		enum class type
		{
			vertex,
			fragment
		};

	public:
		shader() = default;
		shader(type shader_type, const std::string& source);
		~shader();

		shader(const shader&) = delete;
		shader& operator=(const shader&) = delete;

		shader(shader&&) noexcept;
		shader& operator=(shader&&) noexcept;

		GLuint get_id() const;

	private:
		bool compile();
		std::string get_compilation_info();

	private:
		GLuint m_id = 0;
	};
}