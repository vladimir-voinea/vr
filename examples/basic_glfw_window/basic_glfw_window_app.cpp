#include "basic_glfw_window_app.hpp"

#include "main_loop.hpp"

#include <vr-glfw.hpp>

#include <iostream>

void basic_glfw_window_app::run()
{
	vr::glfw_window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "Basic window example";

	vr::glfw_window window(settings);
	if (window.init())
	{
		main_loop loop(window);

		std::cout << "Window created successfully";
		const auto success = window.run(loop);
	}
	else
	{
		std::cout << "Window could not be created";
	}

	std::cout << std::endl;
}