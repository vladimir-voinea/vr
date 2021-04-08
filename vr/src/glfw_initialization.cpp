#include "glfw_initialization.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace
{
	bool glfw_is_initialized = false;
}

namespace vr {
	bool initialize_glfw_once()
	{
		if (!glfw_is_initialized) {
			const auto result = glfwInit();
			glfw_is_initialized = result == GLFW_TRUE;
		}

		return glfw_is_initialized;
	}

	void deinitialize_glfw()
	{
		glfwTerminate();
	}
}