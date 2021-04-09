#pragma once

#include "vk_validation_layer_settings.hpp"
#include "vk_instance.hpp"

#include <vulkan/vulkan.h>

namespace vr
{
	class vk_validation_layer
	{
	public:
		vk_validation_layer(const vk_instance& instance_handle, vk_validation_layer_settings settings);
		~vk_validation_layer();

		bool init();

	private:
		bool get_create_destroy_functions();

	private:
		const vk_instance& m_instance;
		vk_validation_layer_settings m_settings;
		VkDebugUtilsMessengerEXT m_debug_utils_messenger;

		PFN_vkCreateDebugUtilsMessengerEXT m_create_debug_utils_messenger_function = nullptr;
		PFN_vkDestroyDebugUtilsMessengerEXT m_destroy_debug_utils_messenger_function = nullptr;
	};
}