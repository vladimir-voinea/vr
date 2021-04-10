#include "basic_glfw_window_app.hpp"

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
		std::cout << "Window created successfully";
		const auto success = window.run();
	}
	else
	{
		std::cout << "Window could not be created";
	}

	std::cout << std::endl;
}