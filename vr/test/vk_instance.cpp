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