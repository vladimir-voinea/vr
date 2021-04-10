#pragma once

#include <glfw_window.hpp>

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

#include <glm/glm.hpp>

class main_loop
{
public:
	main_loop(vr::glfw_window& window);
	~main_loop();

	void init();

	void run();

private:
	void calculate_mvp();

private:
	vr::glfw_window& m_window;
	shaders m_shaders;

	GLuint m_mvp_uniform;
	glm::mat2 m_mvp;

	GLuint m_vertex_array;
	GLuint m_vertex_buffer;
	GLuint m_color_buffer;
};
