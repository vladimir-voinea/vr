#include <gtest/gtest.h>

#include <vk_extensions.hpp>

TEST(vk_extensions, get_supported_extensions_no_fail)
{
	EXPECT_NO_THROW(vr::get_supported_extensions());
	EXPECT_NO_FATAL_FAILURE(vr::get_supported_extensions());
}

TEST(vk_extensions, get_supported_extensions_same_as_for_empty_layer)
{
	const auto empty_string_layer_count = vr::get_supported_extensions_for_layer({}).size();
	const auto general_extensions_count = vr::get_supported_extensions().size();

	EXPECT_EQ(general_extensions_count, empty_string_layer_count);
}

TEST(vk_extensions, get_supported_extensions_for_empty_layer_no_fail)
{
	EXPECT_NO_THROW(vr::get_supported_extensions_for_layer({}));
	EXPECT_NO_FATAL_FAILURE(vr::get_supported_extensions_for_layer({}));

	EXPECT_NO_THROW(vr::get_supported_extensions_for_layer(""));
	EXPECT_NO_FATAL_FAILURE(vr::get_supported_extensions_for_layer(""));
}

TEST(vk_extensions, cannot_get_invalid_extension_function)
{
	vr::vk_instance_settings settings;
	vr::vk_instance instance(settings);
	instance.init();

	EXPECT_EQ(nullptr, vr::get_extension_function<PFN_vkCreateDebugUtilsMessengerEXT>(instance, "invalid_extension"));
}