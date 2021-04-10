#pragma once

#include <vector>
#include <string>
#include <optional>

namespace vr
{
	bool is_vulkan_supported();
	std::vector<std::string> get_vulkan_required_extensions();
	void poll_events();
	void wait_events(std::optional<double> timeout = {});
}