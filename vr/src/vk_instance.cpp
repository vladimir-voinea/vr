#include "vk_instance.hpp"

#include <vulkan/vulkan.h>

#include <algorithm>
#include <iterator>

namespace vr
{
	vk_instance::vk_instance(vk_instance_settings settings)
		: m_instance()
		, m_settings(settings)
	{

	}

	vk_instance::~vk_instance()
	{
		vkDestroyInstance(m_instance, nullptr);
	}

	bool vk_instance::init()
	{
		VkApplicationInfo app_info{};
		populate_app_info(app_info);

		std::vector<const char*> extensions;
		std::vector<const char*> layers;
		auto pointer = [](const auto& string) { return string.c_str(); };
		std::transform(m_settings.extensions.begin(), m_settings.extensions.end(), std::back_inserter(extensions), pointer);
		std::transform(m_settings.layers.begin(), m_settings.layers.end(), std::back_inserter(layers), pointer);

		VkInstanceCreateInfo create_info{};
		populate_create_info(create_info, app_info, extensions, layers);

		const VkResult result = vkCreateInstance(&create_info, nullptr, &m_instance);

		return result == VK_SUCCESS;
	}

	void vk_instance::populate_app_info(VkApplicationInfo& app_info) const
	{
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = m_settings.application_name.c_str();
		app_info.applicationVersion = VK_MAKE_VERSION(m_settings.application_version.major, m_settings.application_version.minor, m_settings.application_version.patch);
		app_info.pEngineName = nullptr;
		app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
		app_info.apiVersion = VK_MAKE_VERSION(m_settings.vk_api_version.major, m_settings.vk_api_version.minor, m_settings.vk_api_version.patch);
	}

	void vk_instance::populate_create_info(VkInstanceCreateInfo& create_info, VkApplicationInfo& app_info,
		const std::vector<const char*>& extensions, const std::vector<const char*>& layers) const
	{
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;
		create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();
		create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
		create_info.ppEnabledLayerNames = layers.data();
	}

	VkInstance vk_instance::get_instance_handle() const
	{
		return m_instance;
	}
}