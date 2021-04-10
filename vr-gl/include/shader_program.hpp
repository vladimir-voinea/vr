#pragma once

#include "shader.hpp"

namespace vr::gl 
{
	class shader_program
	{
	public:
		shader_program();
		shader_program(shader vertex_shader, shader fragment_shader);
		~shader_program();

		shader_program(const shader_program&) = delete;
		shader_program& operator=(const shader_program&) = delete;

		shader_program(shader_program&&) noexcept;
		shader_program& operator=(shader_program&&) noexcept;

		bool compile();
		std::string get_compilation_info();

	private:
		bool compile_shaders();

	private:
		GLuint m_program_id = 0;
		shader m_vertex_shader;
		shader m_fragment_shader;
	};
}