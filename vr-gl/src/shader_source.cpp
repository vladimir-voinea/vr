#include "shader_source.hpp"

#include "opengl.h"

#include <stdexcept>
#include <algorithm>

namespace
{
	auto convert_shader_type(vr::gl::shader_source::type type)
	{
		auto result = 0;

		switch (type)
		{
		case vr::gl::shader_source::type::vertex:
		{
			result = GL_VERTEX_SHADER;
			break;
		}
		case vr::gl::shader_source::type::fragment:
		{
			result = GL_FRAGMENT_SHADER;
			break;
		}
		}

		return result;
	}
}

namespace vr::gl
{
	shader_source::shader_source(shader_source::type shader_type, const std::string& source)
	{
		const auto gl_shader_type = convert_shader_type(shader_type);
		m_id = glCreateShader(gl_shader_type);

		if (!m_id)
		{
			throw std::runtime_error("Cannot create shader");
		}

		const char* shader_source_code = source.c_str();
		glShaderSource(m_id, 1, &shader_source_code, nullptr);
		glCompileShader(m_id);
	}

	shader_source::~shader_source()
	{
		glDeleteShader(m_id);
	}

	shader_source::shader_source(shader_source&& other) noexcept
	{
		if (this != &other)
		{
			std::swap(m_id, other.m_id);
		}
	}

	shader_source& shader_source::operator=(shader_source&& other) noexcept
	{
		std::swap(m_id, other.m_id);

		return *this;
	}

	GLuint shader_source::get_id()
	{
		return m_id;
	}

	bool shader_source::was_compiled()
	{
		GLint compilation_status = GL_FALSE;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &compilation_status);

		return compilation_status == GL_TRUE;
	}

	std::string shader_source::get_compilation_info()
	{
		std::string compilation_info;

		int info_length = 0;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			compilation_info.resize(info_length, '-');
			glGetShaderInfoLog(m_id, info_length, nullptr, compilation_info.data());
		}

		return compilation_info;
	}
}