#include "vulkan_support_check_app.hpp"

#include <vr-glfw.hpp>

#include <iostream>

void vulkan_support_check_app::run()
{
	vr::initialize_glfw_once();

	const auto have_vulkan_support = vr::is_vulkan_supported();
	
	if (have_vulkan_support)
	{

		std::cout << "Vulkan is supported\n";
		std::cout << "Required Vulkan extensions for GLFW:\n";

		const auto required_vulkan_extensions = vr::get_vulkan_required_extensions();
		for (const auto& extension : required_vulkan_extensions)
		{
			std::cout << extension << '\n';
		}
	}
	else
	{
		std::cout << "Vulkan is not supported";
	}

	std::cout << std::endl;
}