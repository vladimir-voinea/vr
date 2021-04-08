#include "window.hpp";
#include "glfw_initialization.hpp";

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace vr {
	window::window(window_settings settings)
		: m_settings(settings)
	{
		if (!initialize_glfw_once()) {
			throw std::runtime_error("GLFW initialization failed");
		}

		if (!m_settings.create_opengl_context)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		const bool resizable = m_settings.resizable ? GLFW_TRUE : GLFW_FALSE;
		glfwWindowHint(GLFW_RESIZABLE, resizable);
	}

	window::~window()
	{
		glfwDestroyWindow(m_window);
		deinitialize_glfw();
	}

	bool window::run()
	{
		const auto created = create();
		if (!created) {
			return false;
		}

		while (!close_requested())
		{
			glfwPollEvents();
		}

		return true;
	}

	bool window::create()
	{
		m_window = glfwCreateWindow(m_settings.width, m_settings.height, m_settings.name.c_str(), nullptr, nullptr);

		return m_window != nullptr;
	}

	bool window::close_requested()
	{
		return glfwWindowShouldClose(m_window);
	}
}