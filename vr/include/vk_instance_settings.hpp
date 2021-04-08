#pragma once

#include <vector>
#include <string>

namespace vr
{
	struct vk_instance_settings
	{
		std::string application_name;
		std::vector<std::string> extensions;
	};
}