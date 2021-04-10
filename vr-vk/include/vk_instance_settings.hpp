#pragma once

#include "vk_debug_default_callback.hpp"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace vr::vk
{
	struct version
	{
		uint16_t major = 0;
		uint16_t minor = 0;
		uint16_t patch = 0;
	};

	struct vk_instance_settings
	{
		std::string application_name;
		version application_version;
		std::vector<std::string> extensions;
		std::vector<std::string> layers;
		version vk_api_version = { 1, 0, 0 };
	};
}