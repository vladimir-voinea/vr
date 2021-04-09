#pragma once

#include "vk_debug_default_callback.hpp"

namespace vr
{
	struct vk_validation_layer_settings
	{
		debug_utils_messenger_callback debug_callback;
		void* user_data;
	};
}