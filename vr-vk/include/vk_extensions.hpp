#pragma once

#include "vk_instance.hpp"

#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace vr
{
	std::vector<VkExtensionProperties> get_supported_extensions();

	std::vector<VkExtensionProperties> get_supported_extensions_for_layer(const std::string& layer_name);

	template<typename FunctionType>
	FunctionType get_extension_function(const vk_instance& instance, const std::string& function_name)
	{
		auto raw_function = vkGetInstanceProcAddr(instance.get_instance_handle(), function_name.c_str());
		auto func = reinterpret_cast<FunctionType>(raw_function);

		return func;
	}
}