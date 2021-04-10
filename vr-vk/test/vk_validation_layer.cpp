#include <gtest/gtest.h>

#include <vk_instance.hpp>
#include <vk_instance_settings.hpp>
#include <vk_validation_layer.hpp>
#include <vk_validation_layer_settings.hpp>

VKAPI_ATTR VkBool32 VKAPI_CALL test_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data)
{
	std::cout << callback_data->pMessage << '\n';
	return VkBool32(VK_FALSE);
}

TEST(vk_validation_layer, can_init_with_validation_extension)
{
	vr::vk_instance_settings settings;
	settings.extensions = { "VK_EXT_debug_utils" };
	settings.layers = { "VK_LAYER_KHRONOS_validation" };

	vr::vk_instance instance(settings);

	EXPECT_TRUE(instance.init());
}

TEST(vk_validation_layer, validation_layer_creation)
{
	vr::vk_instance_settings instance_settings;
	instance_settings.extensions = { "VK_EXT_debug_utils" };
	instance_settings.layers = { "VK_LAYER_KHRONOS_validation" };
	vr::vk_instance instance(instance_settings);
	instance.init();

	vr::vk_validation_layer_settings validation_settings;
	validation_settings.debug_callback = test_debug_callback;
	validation_settings.user_data = nullptr;

	vr::vk_validation_layer validation_layer(instance, validation_settings);
	EXPECT_TRUE(validation_layer.init());
}

// initialization will always succeed. we cannot validate this yet
TEST(vk_validation_layer, DISABLED_validation_layer_creation_with_null_debug_callback_fails)
{

	vr::vk_instance_settings instance_settings;
	instance_settings.extensions = { "VK_EXT_debug_utils" };
	instance_settings.layers = { "VK_LAYER_KHRONOS_validation" };
	vr::vk_instance instance(instance_settings);
	instance.init();

	vr::vk_validation_layer_settings validation_settings;
	validation_settings.debug_callback = nullptr;
	validation_settings.user_data = nullptr;

	vr::vk_validation_layer validation_layer(instance, validation_settings);
	EXPECT_FALSE(validation_layer.init());
}
