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
		shader(type shader_type, const std::string& source);
		~shader();

	private:
		//GLuint m_shader;
	};
}