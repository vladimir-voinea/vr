#pragma once

#include <vector>
#include <string>

namespace vr
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
		version vk_api_version;
	};
}