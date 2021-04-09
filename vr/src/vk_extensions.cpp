#include "vk_extensions.hpp"

#include <optional>
#include <stdexcept>

namespace
{
	std::vector<VkExtensionProperties> get_extensions(const std::optional<std::string> layer_name)
	{
		uint32_t extension_count = 0;
		if (const auto got_count = vkEnumerateInstanceExtensionProperties(layer_name ? layer_name->c_str() : nullptr, &extension_count, nullptr); got_count == VK_SUCCESS)
		{
			std::vector<VkExtensionProperties> extensions(extension_count);
			if (const auto got_extensions = vkEnumerateInstanceExtensionProperties(layer_name ? layer_name->c_str() : nullptr, &extension_count, extensions.data()); got_extensions == VK_SUCCESS)
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

namespace vr
{
	std::vector<VkExtensionProperties> get_supported_extensions()
	{
		return get_extensions({});
	}

	std::vector <VkExtensionProperties> get_supported_extensions_for_layer(const std::string& layer_name)
	{
		return get_extensions(layer_name);
	}
}