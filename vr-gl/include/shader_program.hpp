#pragma once

#include "shader.hpp"

namespace vr::gl 
{
	class shader_program
	{
	public:
		shader_program() = default;
		shader_program(const shader& vertex_shader, const shader& fragment_shader);
		~shader_program();

		shader_program(const shader_program&) = delete;
		shader_program& operator=(const shader_program&) = delete;

		shader_program(shader_program&&) noexcept;
		shader_program& operator=(shader_program&&) noexcept;

		GLuint get_id() const;

	private:

		bool link();
		std::string get_linkage_info();

	private:
		GLuint m_program_id = 0;
		const shader* m_vertex_shader = nullptr;
		const shader* m_fragment_shader = nullptr;
	};
}
