#include <gtest/gtest.h>

#include <vk_instance.hpp>
#include <vk_instance_settings.hpp>

TEST(vk_instance, can_init_instance_with_empty_settings)
{
	{
		vr::vk_instance_settings empty_settings;
		vr::vk_instance instance(empty_settings);

		EXPECT_TRUE(instance.init());
	}

	{
		vr::vk_instance_settings empty_settings;
		vr::vk_instance instance(empty_settings);

		EXPECT_NO_FATAL_FAILURE(instance.init());
	}
}

TEST(vk_instance, can_init_with_validation_layer)
{
	vr::vk_instance_settings settings;
	settings.layers = { "VK_LAYER_KHRONOS_validation" };

	vr::vk_instance instance(settings);

	EXPECT_TRUE(instance.init());
}

TEST(vk_instance, cannot_init_with_inexistent_layer)
{
	vr::vk_instance_settings settings;
	settings.layers = { "inexistent_layer" };

	vr::vk_instance instance(settings);

	EXPECT_FALSE(instance.init());
}

TEST(vk_instance, cannot_init_with_inexistent_extension)
{
	vr::vk_instance_settings settings;
	settings.extensions = { "inexistent_extension" };

	vr::vk_instance instance(settings);

	EXPECT_FALSE(instance.init());
}