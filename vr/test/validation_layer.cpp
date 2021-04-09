#include <gtest/gtest.h>

#include <vk_instance.hpp>
#include <vk_instance_settings.hpp>

TEST(validation_layer, can_init_with_validation_layer)
{
	vr::vk_instance_settings settings;
	settings.layers = { "VK_LAYER_KHRONOS_validation" };

	vr::vk_instance instance(settings);

	EXPECT_TRUE(instance.init());
}

TEST(validation_layer, can_init_with_debug_utils_extension_no_layers)
{
	vr::vk_instance_settings settings;
	settings.extensions = { "VK_EXT_debug_utils" };

	vr::vk_instance instance(settings);

	EXPECT_TRUE(instance.init());
}