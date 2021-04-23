#include "basic_glfw_window_app.hpp"

#include "main_loop.hpp"

#include <vr-glfw.hpp>

#include <iostream>

void basic_glfw_window_app::run()
{
	vr::glfw::window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "Basic window example";

	vr::glfw::window window(settings);
	if (window.init())
	{
		std::cout << "Window created successfully";

		main_loop loop(window);
		loop.run();
	}
	else
	{
		std::cout << "Window could not be created";
	}

	std::cout << std::endl;
}