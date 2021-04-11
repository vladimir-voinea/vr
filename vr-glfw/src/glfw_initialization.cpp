#include "glfw_initialization.hpp"

#include <GLFW/glfw3.h>

namespace
{
	bool glfw_is_initialized = false;
}

namespace vr::glfw {
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