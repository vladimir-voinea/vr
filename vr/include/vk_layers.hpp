#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace vr
{
	std::vector<VkLayerProperties> get_supported_layers();
}