#pragma once

#include "glfw_window.hpp"
#include "glfw_keyboard_data.hpp"
#include "glfw_keyboard_listener.hpp"
#include <string>

namespace vr::glfw
{
	class keyboard
	{
	public:
		keyboard(window& window);

		void set_listener(keyboard_listener* listener);
		void set_sticky_keys(bool value);
		key_action get_key_state(const key& key);

		static int convert_to_raw(key key);

	private:
		friend void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void key_callback(int key, int scancode, int action, int mods);

		friend void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);
		void char_callback(unsigned int codepoint);

	private:
		window& m_window;
		keyboard_listener* m_listener = nullptr;
	};
}