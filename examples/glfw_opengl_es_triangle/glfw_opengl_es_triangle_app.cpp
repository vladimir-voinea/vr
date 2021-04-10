#include "glfw_opengl_es_triangle_app.hpp"

#include <GL/glew.h>

#include <vr-glfw.hpp>
#include <vr-gl.hpp>

#include <iostream>

void glfw_opengl_es_triangle_app::run()
{
	vr::opengl_context opengl_context_settings;
	opengl_context_settings.api = vr::opengl_context_api::opengl_es;
	opengl_context_settings.context_version = vr::version{ 2, 0 };
	opengl_context_settings.profile = vr::opengl_profile::core;
	opengl_context_settings.foward_compatible = true;

	vr::glfw_window_settings settings;
	settings.width = 800;
	settings.height = 600;
	settings.name = "GLFW OpenGL ES 2.0 Triangle";
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