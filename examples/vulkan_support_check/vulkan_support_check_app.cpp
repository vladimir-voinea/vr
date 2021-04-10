#include "vulkan_support_check_app.hpp"

#include <vr-glfw.hpp>
#include <vr-vk.hpp>

#include <algorithm>
#include <unordered_set>
#include <iostream>

bool check_required_extensions_support(const std::vector<std::string>& required_extensions, std::vector<VkExtensionProperties> supported_extensions)
{
	std::unordered_set<std::string> supported_extension_names;

	std::transform(supported_extensions.begin(), supported_extensions.end(), std::inserter(supported_extension_names, supported_extension_names.begin()),
		[](const auto& extension_properties) {
			return extension_properties.extensionName;
		});

	const bool all_supported = std::all_of(required_extensions.cbegin(), required_extensions.cend(),
		[&supported_extension_names](const auto& extension) {
			return supported_extension_names.contains(extension);
		});

	return all_supported;
}

void vulkan_support_check_app::run()
{
	try {
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

			std::cout << "\nAvailable extensions:\n";

			const auto available_extensions = vr::vk::get_supported_extensions();
			for (const auto& extension : available_extensions)
			{
				std::cout << extension.extensionName << '\n';
			}

			std::cout << '\n';

			if (check_required_extensions_support(required_vulkan_extensions, available_extensions))
			{
				std::cout << "All required extensions are supported\n";
			}
			else
			{
				std::cout << "Required extensions are not supported\n";
			}
		}
		else
		{
			std::cout << "Vulkan is not supported\n";
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception caught: " << ex.what();
	}

	std::cout << std::endl;
}