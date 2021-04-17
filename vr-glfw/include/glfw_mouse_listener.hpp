#pragma once

#include "glfw_mouse_data.hpp"

namespace vr::glfw
{
	class mouse_listener
	{
	public:
		virtual void on_position_event(const mouse_position& position) = 0;
		virtual void on_button_event(mouse_button button, mouse_action action) = 0;
		virtual void on_scroll_event(const mouse_scroll& scroll) = 0;
		virtual void on_enter_event(bool entered) = 0;
	};
}