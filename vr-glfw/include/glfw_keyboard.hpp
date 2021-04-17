#pragma once

#include "glfw_window.hpp"
#include "glfw_keyboard_data.hpp"
#include "glfw_keyboard_listener.hpp"

namespace vr::glfw
{
	class keyboard
	{
	public:
		keyboard(window& window);

		void set_listener(keyboard_listener* listener);
		void set_sticky_keys(bool value);
		key_action get_key_state(const key& key);

	private:
		friend void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void key_callback(int key, int scancode, int action, int mods);

	private:
		window& m_window;
		keyboard_listener* m_listener = nullptr;
	};
}