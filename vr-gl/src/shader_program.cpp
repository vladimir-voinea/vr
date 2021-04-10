#include "shader_program.hpp"

#include "opengl.h"

#include <stdexcept>

namespace vr::gl
{
	shader_program::shader_program(const shader& vertex_shader, const shader& fragment_shader)
		: m_vertex_shader(&vertex_shader)
		, m_fragment_shader(&fragment_shader)
	{
		m_program_id = glCreateProgram();
		if (!m_program_id)
		{
			throw std::runtime_error("Cannot create shader program");
		}

		if (!link())
		{
			const std::string message = "Cannot link shader program: " + get_linkage_info();

			throw std::runtime_error(message);
		}
	}

	shader_program::~shader_program()
	{
		if (m_vertex_shader)
		{
			glDetachShader(m_program_id, m_vertex_shader->get_id());
		}
		
		if (m_fragment_shader)
		{
			glDetachShader(m_program_id, m_fragment_shader->get_id());
		}

		glDeleteProgram(m_program_id);
	}

	shader_program::shader_program(shader_program&& other) noexcept
	{
		*this = std::move(other);
	}

	shader_program& shader_program::operator=(shader_program&& other) noexcept
	{
		if (this != &other)
		{
			std::swap(m_program_id, other.m_program_id);
			std::swap(m_vertex_shader, other.m_vertex_shader);
			std::swap(m_fragment_shader, other.m_fragment_shader);
		}

		return *this;
	}

	GLuint shader_program::get_id() const
	{
		return m_program_id;
	}

	bool shader_program::link()
	{
		glAttachShader(m_program_id, m_vertex_shader->get_id());
		glAttachShader(m_program_id, m_fragment_shader->get_id());

		glLinkProgram(m_program_id);

		GLint result = GL_FALSE;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);

		return result == GL_TRUE;
	}

	std::string shader_program::get_linkage_info()
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
}