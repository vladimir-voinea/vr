#include "shader_program.hpp"

#include "vr-opengl.h"

#include <iostream>
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
			std::swap(m_uniform_names, other.m_uniform_names);
			std::swap(m_attribute_names, other.m_attribute_names);
		}

		return *this;
	}

	GLuint shader_program::get_id() const
	{
		return m_program_id;
	}

	const std::vector<std::string>& shader_program::get_uniform_names() const
	{
		return m_uniform_names;
	}

	const std::vector<std::string>& shader_program::get_attribute_names() const
	{
		return m_attribute_names;
	}

	bool shader_program::link()
	{
		glAttachShader(m_program_id, m_vertex_shader->get_id());
		glAttachShader(m_program_id, m_fragment_shader->get_id());

		glLinkProgram(m_program_id);

		GLint result = GL_FALSE;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);

		if (result == GL_TRUE)
		{
			store_uniform_and_attribute_names();
			return true;
		}

		return false;
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
	
	void shader_program::store_uniform_and_attribute_names()
	{
		GLint uniforms = 0;
		glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &uniforms);
		for (auto i = 0u; i < uniforms; ++i)
		{
			GLint size;
			GLenum type;
			char buf[GL_ACTIVE_UNIFORM_MAX_LENGTH];
			glGetActiveUniform(m_program_id, i, GL_ACTIVE_UNIFORM_MAX_LENGTH, nullptr, &size, &type, buf);
			m_uniform_names.push_back(buf);
		}

		GLint attributes = 0;
		glGetProgramiv(m_program_id, GL_ACTIVE_ATTRIBUTES, &attributes);
		for (auto i = 0u; i < attributes; ++i)
		{
			GLint size;
			GLenum type;
			char buf[GL_ACTIVE_ATTRIBUTE_MAX_LENGTH];
			glGetActiveAttrib(m_program_id, i, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, nullptr, &size, &type, buf);
			m_attribute_names.push_back(buf);
		}
	}
}