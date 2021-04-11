#include "main_loop.hpp"
#include "cube_data.hpp"
#include "shader_loader.hpp"

#include <glfw_util.hpp>
#define GLEW_STATIC
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

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

main_loop::main_loop(vr::glfw_window& window)
	: m_window(window)
{
	init();
}

main_loop::~main_loop()
{
	glDeleteBuffers(1, &m_color_buffer);
	glDeleteBuffers(1, &m_vertex_buffer);
	glDeleteVertexArrays(1, &m_vertex_array);
}

void main_loop::calculate_matrices_from_inputs()
{
	const auto current_time = glfwGetTime();
	const float delta_time = static_cast<float>(current_time - m_last_timestamp);

	double mouse_x, mouse_y;
	glfwGetCursorPos(m_window.get_handle(), &mouse_x, &mouse_y);

	int window_width, window_height;
	glfwGetWindowSize(m_window.get_handle(), &window_width, &window_height);
	const auto window_middle_horizontal = static_cast<double>(window_width) / 2.0f;
	const auto window_middle_vertical = static_cast<double>(window_height) / 2.0f;

	glfwSetCursorPos(m_window.get_handle(), window_middle_vertical, window_middle_horizontal);

	m_horizontal_angle += static_cast<float>(m_mouse_speed * (window_middle_horizontal - mouse_x));
	m_vertical_angle += static_cast<float>(m_mouse_speed * (window_middle_vertical - mouse_y));

	std::cout << "Vertical angle: " << m_vertical_angle << " horizontal angle: " << m_horizontal_angle << '\n';

	const glm::vec3 direction(std::cos(m_vertical_angle) * std::sin(m_horizontal_angle),
		std::sin(m_vertical_angle),
		std::cos(m_vertical_angle) * std::cos(m_horizontal_angle));

	const glm::vec3 right(std::sin(m_horizontal_angle - glm::pi<float>() / 2.0f),
		0,
		std::cos(m_horizontal_angle - glm::pi<float>() / 2.0f));

	const glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(m_window.get_handle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += direction * delta_time * m_speed;
	}

	if (glfwGetKey(m_window.get_handle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position -= direction * delta_time * m_speed;
	}

	if (glfwGetKey(m_window.get_handle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += right * delta_time * m_speed;
	}

	if (glfwGetKey(m_window.get_handle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position -= right * delta_time * m_speed;
	}

	std::cout << "Position: " << m_position.x << ' ' << m_position.y << ' ' << m_position.z << '\n';

	const auto aspect_ratio = static_cast<float>(window_middle_horizontal / window_middle_vertical);
	m_projection = glm::perspective(glm::radians(45.f), aspect_ratio, 0.1f, 100.0f);
	m_view = glm::lookAt(m_position, m_position + direction, up);
	m_mvp = m_projection * m_view * glm::mat4(1.0f);

	m_last_timestamp = current_time;
}

void main_loop::initialize_controls()
{
	m_position = glm::vec3(-4, 3, -3);
	m_vertical_angle = 0.0f;
	m_horizontal_angle = glm::pi<float>();
	m_speed = 3.0f;
	m_mouse_speed = 0.005f;
	m_last_timestamp = glfwGetTime();
}

void main_loop::init()
{
	initialize_glew();
	m_window.set_sticky_keys(true);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &m_vertex_array);
	glBindVertexArray(m_vertex_array);

	initialize_controls();
	m_shaders = load_shaders();
	m_mvp_uniform = glGetUniformLocation(m_shaders.program.get_id(), "mvp");
	m_position_attribute_location = glGetAttribLocation(m_shaders.program.get_id(), "position");
	m_vertex_color_attribute_location = glGetAttribLocation(m_shaders.program.get_id(), "vertex_color");
	calculate_initial_mvp();

	glGenBuffers(1, &m_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
	constexpr auto vertex_data_size = sizeof(vertex_data);
	glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &m_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);
	constexpr auto color_data_size = sizeof(color_data);
	glBufferData(GL_ARRAY_BUFFER, color_data_size, color_data, GL_STATIC_DRAW);
}

void main_loop::calculate_initial_mvp()
{
	constexpr float field_of_view = glm::radians(45.0f);
	constexpr float aspect_ratio = 4.0f / 3.0f;
	constexpr float near = 0.1f;
	constexpr float far = 100.0f;

	const glm::mat4 projection = glm::perspective(field_of_view, aspect_ratio, near, far);

	const glm::mat4 view = glm::lookAt(m_position,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	const glm::mat4 model = glm::mat4(1.0f);

	m_mvp = projection * view * model;
}

void main_loop::run()
{
	while (!m_window.close_requested())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_shaders.program.get_id());

		calculate_matrices_from_inputs();
		glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, &m_mvp[0][0]);

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
		vr::poll_events();
	}
}