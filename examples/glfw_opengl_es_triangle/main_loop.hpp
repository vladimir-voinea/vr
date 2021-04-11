#pragma once

#include <glfw_window.hpp>

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
	shaders m_shaders;

	GLuint vertex_array;
	GLuint vertex_buffer;
};
