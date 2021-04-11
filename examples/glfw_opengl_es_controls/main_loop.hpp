#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>

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

	void render_scene();

	void run();

private:
	void initialize_controls();
	void process_input();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;

	shaders m_shaders;

	GLuint m_mvp_uniform;
	glm::mat4 m_mvp;
	glm::mat4 m_projection;
	glm::mat4 m_view;

	GLuint m_vertex_array;
	GLuint m_vertex_buffer;
	GLuint m_color_buffer;

	GLuint m_position_attribute_location;
	GLuint m_vertex_color_attribute_location;

	glm::vec3 m_position;
	float m_horizontal_angle;
	float m_vertical_angle;
	float m_speed;
	float m_mouse_speed;
	double m_last_timestamp;
};
