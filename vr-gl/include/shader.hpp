#pragma once

#include "shader_source.hpp"

namespace vr::gl 
{
	class shader
	{
	public:
		shader();
		shader(shader_source vertex_shader, shader_source fragment_shader);
		~shader();

		shader(const shader&) = delete;
		shader& operator=(const shader&) = delete;

		shader(shader&&) noexcept;
		shader& operator=(shader&&) noexcept;

		bool compile();
		std::string get_compilation_info();

	private:
		bool compile_shaders();

	private:
		GLuint m_program_id = 0;
		shader_source m_vertex_shader;
		shader_source m_fragment_shader;
	};
}