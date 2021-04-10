#include "shader.hpp"

#include "opengl.h"

#include <stdexcept>
#include <algorithm>

namespace
{
	auto convert_shader_type(vr::gl::shader::type type)
	{
		auto result = 0;

		switch (type)
		{
		case vr::gl::shader::type::vertex:
		{
			result = GL_VERTEX_SHADER;
			break;
		}
		case vr::gl::shader::type::fragment:
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
	shader::shader(shader::type shader_type, const std::string& source)
	{
		const auto gl_shader_type = convert_shader_type(shader_type);
		m_id = glCreateShader(gl_shader_type);

		if (!m_id)
		{
			throw std::runtime_error("Cannot create shader");
		}

		const char* shader_source_code = source.c_str();
		glShaderSource(m_id, 1, &shader_source_code, nullptr);

		if (!compile())
		{
			const std::string message = "Cannot compile shader: " + get_compilation_info();
			throw std::runtime_error(message);
		}
	}

	shader::~shader()
	{
		glDeleteShader(m_id);
	}

	shader::shader(shader&& other) noexcept
	{
		*this = std::move(other);
	}

	shader& shader::operator=(shader&& other) noexcept
	{
		if (this != &other)
		{
			std::swap(m_id, other.m_id);
		}

		return *this;
	}

	GLuint shader::get_id() const
	{
		return m_id;
	}

	bool shader::compile()
	{
		glCompileShader(m_id);

		GLint compilation_status = GL_FALSE;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &compilation_status);

		return compilation_status == GL_TRUE;
	}

	std::string shader::get_compilation_info()
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