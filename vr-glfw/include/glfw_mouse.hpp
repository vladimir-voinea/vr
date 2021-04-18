#pragma once

#include "glfw_window.hpp"
#include "glfw_mouse_data.hpp"
#include "glfw_mouse_listener.hpp"

namespace vr::glfw
{
	class mouse
	{
	public:
		mouse(window& window);
		mouse(const mouse&) = delete;
		mouse& operator=(const mouse&) = delete;

		void set_listener(mouse_listener* listener);
		void set_sticky_buttons(bool value);
		void set_raw_motion(bool value);

		mouse_position get_position();
		void set_position(const mouse_position& position);

		void set_mode(const mouse_mode& mode);

	private:
		friend void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		void cursor_position_callback(double xpos, double ypos);

		friend void glfw_button_callback(GLFWwindow* window, int button, int action, int mods);
		void button_callback(int button, int action, int mods);

		friend void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void scroll_callback(double xoffset, double yoffset);

		friend void glfw_cursor_enter_callback(GLFWwindow* window, int entered);
		void cursor_enter_callback(int entered);

	private:
		window& m_window;
		mouse_listener* m_listener = nullptr;
	};
}