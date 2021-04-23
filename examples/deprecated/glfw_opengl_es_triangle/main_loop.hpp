#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();

	void run();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	shaders m_shaders;

	GLuint vertex_array;
	GLuint vertex_buffer;
};
