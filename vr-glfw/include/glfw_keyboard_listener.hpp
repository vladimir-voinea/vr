#pragma once

#include "glfw_keyboard_data.hpp"

namespace vr::glfw
{
	class keyboard_listener
	{
	public:
		virtual void on_key_event(key key, key_action state, modifiers mods) = 0;
		virtual void on_char_event(unsigned int codepoint) = 0;
	};
}