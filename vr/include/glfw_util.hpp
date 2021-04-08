#pragma once

#include <vector>
#include <string>

namespace vr
{
	bool is_vulkan_supported();
	std::vector<std::string> get_vulkan_required_extensions();
}