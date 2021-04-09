#include "vk_debug_default_callback.hpp"

#include <iostream>

namespace vr
{
	VKAPI_ATTR VkBool32 VKAPI_CALL default_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data)
	{
		std::cerr << "Debug callback: " << callback_data->pMessage << '\n';

		return VkBool32(VK_FALSE);
	}
}