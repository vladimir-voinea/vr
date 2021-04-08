#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace vr
{
	std::vector<VkExtensionProperties> get_supported_extensions();
}