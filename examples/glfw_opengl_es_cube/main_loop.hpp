#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

#include <glm/glm.hpp>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();

	void run();

private:
	void calculate_mvp();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	shaders m_shaders;

	GLuint m_mvp_uniform;
	glm::mat4 m_mvp;

	GLuint m_vertex_array;
	GLuint m_vertex_buffer;
	GLuint m_color_buffer;

	GLuint m_position_attribute_location;
	GLuint m_vertex_color_attribute_location;
};
