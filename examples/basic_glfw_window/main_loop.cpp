#include "main_loop.hpp"

#include <glfw_util.hpp>

#include <stdexcept>

main_loop::main_loop(vr::glfw_window& window)
	: m_window(window)
{
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