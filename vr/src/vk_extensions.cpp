#include "vk_extensions.hpp"

#include <stdexcept>

namespace vr
{
	std::vector<VkExtensionProperties> get_supported_extensions()
	{
		uint32_t extension_count = 0;
		if (const auto got_count = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr); got_count == VK_SUCCESS)
		{
			std::vector<VkExtensionProperties> extensions(extension_count);
			if (const auto got_extensions = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()); got_extensions == VK_SUCCESS)
			{
				return extensions;
			}
			else
			{
				throw std::runtime_error("Could not get supported extensions");
			}
		}
		else
		{
			throw std::runtime_error("Could not get supported extensions count");
		}
	}
}