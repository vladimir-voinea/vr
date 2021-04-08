#include "basic_window_app.hpp"

#include <vr.hpp>

#include <iostream>

void basic_window_app::run()
{
	vr::window window(800, 600, "Basic window example");

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