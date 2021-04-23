#include "main_loop.hpp"
#include "shader_loader.hpp"

#include <glfw_util.hpp>
#define GLEW_STATIC
#include <gl/glew.h>

#include <stdexcept>

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
{
	init();
}

main_loop::~main_loop()
{
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vertex_array);
}

void main_loop::init()
{
	initialize_glew();
	m_kb.set_sticky_keys(true);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	constexpr GLfloat vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
	};

	m_shaders = load_shaders();

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	const GLsizeiptr data_size = sizeof(vertex_buffer_data);
	glBufferData(GL_ARRAY_BUFFER, data_size, vertex_buffer_data, GL_STATIC_DRAW);
}

void main_loop::run()
{
	while (!m_window.close_requested())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_shaders.program.get_id());

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		m_window.swap_buffers();
		vr::glfw::poll_events();
	}
}