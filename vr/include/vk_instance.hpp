#pragma once

#include "vk_instance_settings.hpp"

#include <vulkan/vulkan.h>

#include <memory>

namespace vr
{
	class vk_instance
	{
	public:
		vk_instance(vk_instance_settings settings);
		~vk_instance();

		bool init();

	private:
		void populate_app_info(VkApplicationInfo& app_info) const;
		void populate_create_info(VkInstanceCreateInfo& create_info, VkApplicationInfo& app_info) const;

	private:
		VkInstance m_instance;
		const vk_instance_settings m_settings;
	};
}