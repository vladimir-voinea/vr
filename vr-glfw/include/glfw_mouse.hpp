#pragma once

#include "glfw_window.hpp"
#include "glfw_mouse_data.hpp"

namespace vr::glfw
{
	class mouse
	{
	public:
		mouse(window& window);
		mouse(const mouse&) = delete;
		mouse& operator=(const mouse&) = delete;

		void set_sticky_buttons(bool value);

		mouse_position get_position();
		void set_position(const mouse_position& position);

		void set_mode(const mouse_mode& mode);

	private:
		friend void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		void cursor_position_callback(double xpos, double ypos);

	private:
		window& m_window;
	};
}