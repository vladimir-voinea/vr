#include "shader.hpp"

#include "opengl.h"

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
		GLuint jet = 0;
		//m_shader = glCreateShader();
	}

	shader::~shader()
	{

	}
}