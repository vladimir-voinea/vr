#include <gtest/gtest.h>

#include <vk_layers.hpp>

#include <string_view>
#include <algorithm>

TEST(vk_layers, validation_layer_supported)
{
	const auto layers = vr::get_supported_layers();
	const bool contains_validation_layer = std::any_of(layers.begin(), layers.end(),
		[](const VkLayerProperties& layer) {
			return std::string_view{ layer.layerName } == "VK_LAYER_KHRONOS_validation"; });


	EXPECT_TRUE(contains_validation_layer);
}
