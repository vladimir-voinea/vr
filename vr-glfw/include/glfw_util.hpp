#pragma once

#include <vector>
#include <string>
#include <optional>

namespace vr::glfw
{
	bool is_vulkan_supported();
	std::vector<std::string> get_vulkan_required_extensions();
	double get_time();
	void poll_events();
	void wait_events(std::optional<double> timeout = {});
}