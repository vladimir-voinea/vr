#include "main_loop.hpp"
#include "shader_loader.hpp"

#include <glfw_util.hpp>
#define GLEW_STATIC
#include <gl/glew.h>

#include <stdexcept>

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
	initialize_glew();

	m_window.set_sticky_keys(true);

	m_shaders = load_shaders();
}

bool main_loop::loop()
{
	vr::poll_events();

	if (m_window.close_requested())
	{
		return false;
	}

	return true;
}