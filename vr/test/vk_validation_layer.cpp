#include <gtest/gtest.h>

#include <vk_instance.hpp>
#include <vk_instance_settings.hpp>

TEST(vk_validation_layer, can_init_with_validation_extension)
{
	{
		vr::vk_instance_settings settings;
		settings.extensions = { "VK_EXT_debug_utils" };

		vr::vk_instance instance(settings);

		EXPECT_TRUE(instance.init());
	}
	{
		vr::vk_instance_settings settings;
		settings.layers = { "VK_LAYER_KHRONOS_validation" };

		vr::vk_instance instance(settings);

		EXPECT_TRUE(instance.init());
	}
	{
		vr::vk_instance_settings settings;
		settings.extensions = { "VK_EXT_debug_utils" };
		settings.layers = { "VK_LAYER_KHRONOS_validation" };

		vr::vk_instance instance(settings);

		EXPECT_TRUE(instance.init());
	}
}
