#include "shader.hpp"

#include "opengl.h"

#include <stdexcept>

namespace vr::gl
{
	shader::shader(shader_source vertex_shader, shader_source fragment_shader)
		: m_vertex_shader(std::move(vertex_shader))
		, m_fragment_shader(std::move(fragment_shader))
	{
		m_program_id = glCreateProgram();
		if (!m_program_id)
		{
			throw std::runtime_error("Cannot create shader program");
		}
	}

	shader::~shader()
	{
		glDeleteProgram(m_program_id);
	}

	shader::shader(shader&& other) noexcept
	{
		*this = std::move(other);
	}

	shader& shader::operator=(shader&& other) noexcept
	{
		if (this != &other)
		{
			std::swap(m_program_id, other.m_program_id);
			std::swap(m_vertex_shader, other.m_vertex_shader);
			std::swap(m_fragment_shader, other.m_fragment_shader);
		}

		return *this;
	}

	bool shader::compile()
	{
		if (compile_shaders())
		{
			glAttachShader(m_program_id, m_vertex_shader.get_id());
			glAttachShader(m_program_id, m_fragment_shader.get_id());
		}

		glLinkProgram(m_program_id);

		GLint result = GL_FALSE;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);

		return result == GL_TRUE;
	}

	std::string shader::get_compilation_info()
	{
		std::string compilation_info;

		int info_length = 0;
		glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			compilation_info.resize(info_length, '-');
			glGetProgramInfoLog(m_program_id, info_length, nullptr, compilation_info.data());
		}

		return compilation_info;
	}

	bool shader::compile_shaders()
	{
		return m_vertex_shader.compile() && m_fragment_shader.compile();
	}
}