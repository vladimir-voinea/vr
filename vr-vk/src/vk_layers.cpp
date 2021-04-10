#include "vk_layers.hpp"

namespace vr::vk
{
	std::vector<VkLayerProperties> get_supported_layers()
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layers.data());

		return layers;
	}
}