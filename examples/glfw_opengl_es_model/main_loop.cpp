#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include "cube_data.hpp"
#include "shader_loader.hpp"

#include <glfw_util.hpp>
#define GLEW_STATIC
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <stdexcept>
#include <iostream>

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void initialize_glew()
{
	glewExperimental = GL_TRUE;
	const auto glew_initialization = glewInit();
	if (glew_initialization != GLEW_OK)
	{
		throw std::runtime_error("Could not initialize glew");
	}
}

main_loop::main_loop(vr::glfw::window& window)
	: m_window(window)
	, m_kb(window)
	, m_mouse(window)
	, m_camera(std::make_unique<vr::gl::perspective_camera>(vr::gl::perspective_camera::settings{
	45.f, static_cast<float>(window.get_size().width) / static_cast<float>(window.get_size().height),
	0.1f,100.f }))
	, m_controls(m_window, *m_camera, m_mouse, m_kb)
{
	init();
}

main_loop::~main_loop()
{
	glDeleteBuffers(1, &m_color_buffer);
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteVertexArrays(1, &m_vertex_array);
}

void main_loop::init()
{
	initialize_glew();
	import_model();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);

	initialize_controls();
	initialize_position();

	m_shaders = load_shaders();
	m_mvp_uniform = glGetUniformLocation(m_shaders.program.get_id(), "mvp");
	m_position_attribute_location = glGetAttribLocation(m_shaders.program.get_id(), "position");
	m_vertex_color_attribute_location = glGetAttribLocation(m_shaders.program.get_id(), "vertex_color");

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	constexpr auto vertex_data_size = sizeof(vertex_data);
	glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
	constexpr auto color_data_size = sizeof(color_data);
	glBufferData(GL_ARRAY_BUFFER, color_data_size, color_data, GL_STATIC_DRAW);

	m_last_timestamp = vr::glfw::get_time();
}

void main_loop::import_model()
{
	constexpr auto path = "data/models/suzanne.obj";

	m_suzanne = m_asset_importer.ReadFile(path, aiPostProcessSteps::aiProcess_ValidateDataStructure);

	if (!m_suzanne)
	{
		const std::string message = "Could not import model: " + std::string(m_asset_importer.GetErrorString());
		throw std::runtime_error(message);
	}
}

void main_loop::initialize_controls()
{
	m_kb.set_sticky_keys(true);
	m_mouse.set_sticky_buttons(true);
	m_mouse.set_mode(vr::glfw::mouse::mode::disabled);
}

void main_loop::initialize_position()
{
	m_camera->set_position({ -8.96424, 2.70909, 4.2585 });
	m_camera->set_direction({ 0.827756, -0.307191, -0.469526 });
}

void main_loop::process_input()
{
	const auto current_time = vr::glfw::get_time();
	const float delta_time = static_cast<float>(current_time - m_last_timestamp);

	m_controls.process_events(delta_time);

	if (m_kb.get_key_state(vr::glfw::keyboard::key::escape) == vr::glfw::keyboard::state::press)
	{
		m_window.request_close();
	}

	m_last_timestamp = current_time;
}

void main_loop::print_state()
{
	std::cout << "Position: " 
		<< m_camera->get_position().x << ", "
		<< m_camera->get_position().y << ", " 
		<< m_camera->get_position().z << '\n';

	std::cout << "Direction: " << 
		m_camera->get_direction().x << ", " 
		<< m_camera->get_direction().y << ", " 
		<< m_camera->get_direction().z << '\n';
}

void main_loop::render_scene()
{
	const auto projection_matrix = m_camera->get_projection_matrix();
	const auto view_matrix = m_controls.get_view_matrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_shaders.program.get_id());

	const auto mvp = projection_matrix * view_matrix * glm::mat4(1.0f);
	glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(m_position_attribute_location);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(m_vertex_color_attribute_location);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glDisableVertexAttribArray(m_vertex_color_attribute_location);
	glDisableVertexAttribArray(m_position_attribute_location);

	m_window.swap_buffers();
}

void main_loop::run()
{
	while (!m_window.close_requested())
	{
		if (m_window.has_focus())
		{
			process_input();
			print_state();
			render_scene();
		}

		vr::glfw::poll_events();
	}
}