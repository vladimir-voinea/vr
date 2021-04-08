#include "window.hpp";
#include "glfw_initialization.hpp";

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace vr {
	window::window(int width, int height, std::string name)
		: m_width(width)
		, m_height(height)
		, m_name(name)
	{
		if (!initialize_glfw_once()) {
			throw std::runtime_error("GLFW initialization failed");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
		m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);

		return m_window != nullptr;
	}

	bool window::close_requested()
	{
		return glfwWindowShouldClose(m_window);
	}
}