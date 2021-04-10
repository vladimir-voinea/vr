#include "glfw_util.hpp"
#include "glfw_initialization.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace vr
{
	bool is_vulkan_supported()
	{
		return glfwVulkanSupported() == GLFW_TRUE;
	}

	std::vector<std::string> get_vulkan_required_extensions()
	{
		std::vector<std::string> result;

		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
		
		if (!glfw_extensions || !glfw_extension_count)
		{
			throw std::runtime_error("Could not get required Vulkan extensions for GLFW");
		}

		for (auto i = 0u; i < glfw_extension_count; ++i)
		{
			result.emplace_back(glfw_extensions[i]);
		}

		return result;
	}

	void poll_events()
	{
		glfwPollEvents();
	}

	void wait_events(std::optional<double> timeout)
	{
		if (!timeout)
		{
			glfwWaitEvents();
		}
		else
		{
			glfwWaitEventsTimeout(timeout);
		}
	}
}