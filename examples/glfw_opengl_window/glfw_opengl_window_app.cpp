#include "glfw_opengl_window_app.hpp"

#include <vr-glfw.hpp>

#include <iostream>

void glfw_opengl_window_app::run()
{
	vr::opengl_context opengl_context_settings;
	opengl_context_settings.api = vr::opengl_context_api::opengl;
	opengl_context_settings.context_version = vr::version{ 4, 6 };
	opengl_context_settings.profile = vr::opengl_profile::core;
	opengl_context_settings.foward_compatible = true;

	vr::glfw_window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "OpenGL window example";
	settings.opengl_context = opengl_context_settings;

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