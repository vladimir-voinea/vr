#pragma once

#include <string>

namespace vr::gl
{
	class opengl_shader
	{
	public:
		opengl_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

		const std::string& get_vertex_shader_source() const;
		const std::string& get_fragment_shader_source() const;

	private:
		std::string m_vshader;
		std::string m_fshader;
	};
}