#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace vr
{
	std::vector<VkExtensionProperties> get_supported_extensions();

	std::vector<VkExtensionProperties> get_supported_extensions_for_layer(const std::string& layer_name);
}