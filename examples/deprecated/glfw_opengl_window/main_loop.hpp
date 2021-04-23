#pragma once

#include <glfw_window.hpp>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);

	void run();

private:
	vr::glfw::window& m_window;
};
