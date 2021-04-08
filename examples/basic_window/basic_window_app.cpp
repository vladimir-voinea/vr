#include "basic_window_app.hpp"

#include <vr.hpp>

#include <iostream>

void basic_window_app::run()
{
	vr::window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "Basic window example";
	settings.create_opengl_context = false;
	settings.resizable = false;

	vr::window window(settings);

	const auto success = window.run();

	if (success)
	{
		std::cout << "Window created successfully";
	}
	else
	{
		std::cout << "Window could not be created";
	}

	std::cout << std::endl;
}