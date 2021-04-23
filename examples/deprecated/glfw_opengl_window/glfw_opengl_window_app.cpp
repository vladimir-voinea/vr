#include "glfw_opengl_window_app.hpp"

#include "main_loop.hpp"

#include <vr-glfw.hpp>

#include <iostream>

void glfw_opengl_window_app::run()
{
	vr::glfw::opengl_context opengl_context_settings;
	opengl_context_settings.api = vr::glfw::opengl_context_api::opengl;
	opengl_context_settings.context_version = vr::glfw::version{ 4, 6 };
	opengl_context_settings.profile = vr::glfw::opengl_profile::core;
	opengl_context_settings.foward_compatible = true;

	vr::glfw::window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "OpenGL window example";
	settings.opengl_context = opengl_context_settings;

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