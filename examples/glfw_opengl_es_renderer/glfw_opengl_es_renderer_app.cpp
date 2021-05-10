#include "glfw_opengl_es_renderer_app.hpp"
#include "preamble.hpp"

#include <vr-glfw.hpp>
#include <vr-gl.hpp>

#include <spdlog/spdlog.h>

void glfw_opengl_es_renderer_app::run()
{
	try
	{
		vr::glfw::opengl_context opengl_context_settings;
		opengl_context_settings.api = vr::glfw::opengl_context_api::opengl_es;
		opengl_context_settings.context_version = vr::glfw::version{ 2, 0 };
		opengl_context_settings.profile = vr::glfw::opengl_profile::core;
		opengl_context_settings.foward_compatible = true;

		vr::glfw::window_settings settings;
		settings.width = 1920;
		settings.height = 1080;
		settings.full_screen = false;
		settings.name = "GLFW OpenGL ES 2.0 Renderer";
		settings.opengl_context = opengl_context_settings;
		settings.resizable = true;
		settings.msaa_samples = 4;

		vr::glfw::window window(settings);
		if (window.init())
		{
			spdlog::info("Window created successfully");

			preamble glfw_preamble(window);
			glfw_preamble.run_loop();
		}
		else
		{
			spdlog::error("Window could not be created");
		}

	}
	catch (const std::exception& e)
	{
		spdlog::error("Exception: {0}", e.what());
	}
}