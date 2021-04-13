#include "glfw_opengl_es_mesh_app.hpp"
#include "main_loop.hpp"

#include <vr-glfw.hpp>
#include <vr-gl.hpp>

#include <iostream>

void glfw_opengl_es_model_app::run()
{
	try
	{
		vr::glfw::opengl_context opengl_context_settings;
		opengl_context_settings.api = vr::glfw::opengl_context_api::opengl_es;
		opengl_context_settings.context_version = vr::glfw::version{ 2, 0 };
		opengl_context_settings.profile = vr::glfw::opengl_profile::core;
		opengl_context_settings.foward_compatible = true;

		vr::glfw::window_settings settings;
		settings.width = 1024;
		settings.height = 768;
		settings.name = "GLFW OpenGL ES 2.0 Cube";
		settings.opengl_context = opengl_context_settings;
		settings.resizable = false;
		settings.msaa_samples = 4;

		vr::glfw::window window(settings);
		if (window.init())
		{
			std::cout << "Window created successfully\n";

			main_loop loop(window);
			loop.run();
		}
		else
		{
			std::cout << "Window could not be created\n";
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	std::cout << std::endl;
}