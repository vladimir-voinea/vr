#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

#include <camera.hpp>
#include "user_controls.hpp"

#include <glm/glm.hpp>

#include <memory>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();
	void run();

private:
	void initialize_controls();
	void initialize_position();

	void process_input();
	void print_state();
	void render_scene();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;

	std::unique_ptr<vr::gl::camera> m_camera;
	user_controls m_controls;
	shaders m_shaders;
	
	double m_last_timestamp;

	GLuint m_mvp_uniform;

	GLuint m_vertex_array;
	GLuint m_vertex_buffer;
	GLuint m_color_buffer;

	GLuint m_position_attribute_location;
	GLuint m_vertex_color_attribute_location;

};
