#include "vk_validation_layer.hpp"

#include <vk_extensions.hpp>

#include <iostream>

namespace vr
{
	vk_validation_layer::vk_validation_layer(const vk_instance& instance, vk_validation_layer_settings settings)
		: m_instance(instance)
		, m_settings(settings)
		, m_debug_utils_messenger()
	{}

	vk_validation_layer::~vk_validation_layer()
	{
		if (m_destroy_debug_utils_messenger_function)
		{
			m_destroy_debug_utils_messenger_function(m_instance.get_instance_handle(), m_debug_utils_messenger, nullptr);
		}
	}

	bool vk_validation_layer::init()
	{
		const auto have_create_destroy_functions = get_create_destroy_functions();

		if (have_create_destroy_functions)
		{
			VkDebugUtilsMessengerCreateInfoEXT create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			create_info.pfnUserCallback = m_settings.debug_callback;
			create_info.pUserData = m_settings.user_data;

			if (m_create_debug_utils_messenger_function(m_instance.get_instance_handle(), &create_info, nullptr, &m_debug_utils_messenger) == VK_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	bool vk_validation_layer::get_create_destroy_functions()
	{
		m_create_debug_utils_messenger_function = get_extension_function<decltype(m_create_debug_utils_messenger_function)>(m_instance, "vkCreateDebugUtilsMessengerEXT");
		m_destroy_debug_utils_messenger_function = get_extension_function<decltype(m_destroy_debug_utils_messenger_function)>(m_instance, "vkDestroyDebugUtilsMessengerEXT");

		return m_create_debug_utils_messenger_function != nullptr &&
			m_destroy_debug_utils_messenger_function != nullptr;
	}
}