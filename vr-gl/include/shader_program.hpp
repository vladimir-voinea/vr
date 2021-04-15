#pragma once

#include "shader.hpp"

#include <vector>
#include <string>

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
		const std::vector<std::string>& get_uniform_names() const;
		const std::vector<std::string>& get_attribute_names() const;

	private:

		bool link();
		std::string get_linkage_info();
		void store_uniform_and_attribute_names();

	private:
		GLuint m_program_id = 0;
		const shader* m_vertex_shader = nullptr;
		const shader* m_fragment_shader = nullptr;
		std::vector<std::string> m_uniform_names;
		std::vector<std::string> m_attribute_names;
	};
}
