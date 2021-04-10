#pragma once

#include <glfw_window.hpp>

class main_loop 
{
public:
	main_loop(vr::glfw_window& window);

	void run();

private:
	vr::glfw_window& m_window;
};
