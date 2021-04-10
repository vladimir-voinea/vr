#pragma once

#include "opengl.h"

#include <string>

namespace vr::gl
{
	class shader_source
	{
	public:
		enum class type
		{
			vertex,
			fragment
		};

	public:
		shader_source(type shader_type, const std::string& source);
		~shader_source();

		shader_source(const shader_source&) = delete;
		shader_source& operator=(const shader_source&) = delete;

		shader_source(shader_source&&) noexcept;
		shader_source& operator=(shader_source&&) noexcept;

		GLuint get_id();

		bool was_compiled();
		std::string get_compilation_info();

	private:
		GLuint m_id = 0;
	};
}