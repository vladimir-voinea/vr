#include "main_loop.hpp"


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
}

bool main_loop::loop()
{
	return true;
}