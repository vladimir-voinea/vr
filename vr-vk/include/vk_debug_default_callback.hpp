#pragma once

#include <vulkan/vulkan.h>

namespace vr::vk
{
	using debug_utils_messenger_callback = PFN_vkDebugUtilsMessengerCallbackEXT;

	VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback( VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data);
}