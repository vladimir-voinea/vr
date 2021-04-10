#include "main_loop.hpp"

#include <glfw_util.hpp>

#include <stdexcept>

main_loop::main_loop(vr::glfw_window& window)
	: m_window(window)
{
}

void main_loop::run()
{
	while (!m_window.close_requested())
	{
		vr::poll_events();
	}
}