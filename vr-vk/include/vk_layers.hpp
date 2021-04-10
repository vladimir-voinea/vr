#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace vr::vk
{
	std::vector<VkLayerProperties> get_supported_layers();
}