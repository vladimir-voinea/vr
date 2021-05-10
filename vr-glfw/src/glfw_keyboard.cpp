#include "glfw_keyboard.hpp"
#include "glfw_user_pointer.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <unordered_map>
#include <string>
#include <stdexcept>

namespace
{
	using k = vr::glfw::key;
	const std::unordered_map<k, int> key_to_glfw_mapping = {
		{ k::a, GLFW_KEY_A },
		{ k::b, GLFW_KEY_B },
		{ k::c, GLFW_KEY_C },
		{ k::d, GLFW_KEY_D },
		{ k::e, GLFW_KEY_E },
		{ k::f, GLFW_KEY_F },
		{ k::g, GLFW_KEY_G },
		{ k::h, GLFW_KEY_H },
		{ k::i, GLFW_KEY_I },
		{ k::j, GLFW_KEY_J },
		{ k::k, GLFW_KEY_K },
		{ k::l, GLFW_KEY_L },
		{ k::m, GLFW_KEY_M },
		{ k::n, GLFW_KEY_N },
		{ k::o, GLFW_KEY_O },
		{ k::p, GLFW_KEY_P },
		{ k::q, GLFW_KEY_Q },
		{ k::r, GLFW_KEY_R },
		{ k::s, GLFW_KEY_S },
		{ k::t, GLFW_KEY_T },
		{ k::u, GLFW_KEY_U },
		{ k::v, GLFW_KEY_V },
		{ k::w, GLFW_KEY_W },
		{ k::x, GLFW_KEY_X },
		{ k::y, GLFW_KEY_Y },
		{ k::z, GLFW_KEY_Z },
		{ k::space, GLFW_KEY_SPACE},
		{ k::left_ctrl, GLFW_KEY_LEFT_CONTROL },
		{ k::left_shift, GLFW_KEY_LEFT_SHIFT },
		{ k::alt, GLFW_KEY_LEFT_ALT },
		{ k::super, GLFW_KEY_LEFT_SUPER },
		{ k::escape, GLFW_KEY_ESCAPE },
		{ k::backspace, GLFW_KEY_BACKSPACE }
	};

	const std::unordered_map<int, k> glfw_to_key_mapping = {
	{ GLFW_KEY_A, k::a },
	{ GLFW_KEY_B, k::b },
	{ GLFW_KEY_C, k::c },
	{ GLFW_KEY_D, k::d },
	{ GLFW_KEY_E, k::e },
	{ GLFW_KEY_F, k::f },
	{ GLFW_KEY_G, k::g },
	{ GLFW_KEY_H, k::h },
	{ GLFW_KEY_I, k::i },
	{ GLFW_KEY_J, k::j },
	{ GLFW_KEY_K, k::k },
	{ GLFW_KEY_L, k::l },
	{ GLFW_KEY_M, k::m },
	{ GLFW_KEY_N, k::n },
	{ GLFW_KEY_O, k::o },
	{ GLFW_KEY_P, k::p },
	{ GLFW_KEY_Q, k::q },
	{ GLFW_KEY_R, k::r },
	{ GLFW_KEY_S, k::s },
	{ GLFW_KEY_T, k::t },
	{ GLFW_KEY_U, k::u },
	{ GLFW_KEY_V, k::v },
	{ GLFW_KEY_W, k::w },
	{ GLFW_KEY_X, k::x },
	{ GLFW_KEY_Y, k::y },
	{ GLFW_KEY_Z, k::z },
	{ GLFW_KEY_SPACE, k::space },
	{ GLFW_KEY_LEFT_CONTROL, k::left_ctrl },
	{ GLFW_KEY_LEFT_SHIFT, k::left_shift },
	{ GLFW_KEY_LEFT_ALT, k::alt },
	{ GLFW_KEY_LEFT_SUPER, k::super },
	{ GLFW_KEY_ESCAPE, k::escape },
	{ GLFW_KEY_BACKSPACE, k::backspace }
	};

	int enum_to_glfw_key(const k& key)
	{
		auto it = key_to_glfw_mapping.find(key);
		if (it == key_to_glfw_mapping.end())
		{
			std::string message = "Keyboard key  enum value " +
				std::to_string(static_cast<uint16_t>(key)) + " has no mapping";
			throw std::runtime_error(message);
		}

		return it->second;
	}

	k glfw_key_to_enum(const int glfw_key)
	{
		auto it = glfw_to_key_mapping.find(glfw_key);
		if (it == glfw_to_key_mapping.end())
		{
			std::string message = "Keyboard key  enum value " +
				std::to_string(glfw_key) + " has no mapping";
			throw std::runtime_error(message);
		}

		return it->second;
	}

	k char_to_enum(const char c)
	{
		if (c >= static_cast<char>(k::a) &&
			c <= static_cast<char>(k::z))
		{
			return static_cast<k>(c);
		}
		else
		{
			throw std::runtime_error("Unsupported key");
		}
	}
}


namespace vr::glfw
{
	keyboard::keyboard(window& window)
		: m_window(window)
	{
		static_cast<user_pointer*>(glfwGetWindowUserPointer(window.get_handle()))->keyboard = this;
		glfwSetKeyCallback(window.get_handle(), glfw_key_callback);
		glfwSetCharCallback(window.get_handle(), glfw_char_callback);
	}

	void keyboard::set_listener(keyboard_listener* listener)
	{
		m_listener = listener;
	}

	void keyboard::set_sticky_keys(bool value)
	{
		glfwSetInputMode(m_window.get_handle(), GLFW_STICKY_KEYS, value ? GL_TRUE : GL_FALSE);
	}

	key_action keyboard::get_key_state(const key& key)
	{
		const auto glfw_key = enum_to_glfw_key(key);
		const auto state = glfwGetKey(m_window.get_handle(), glfw_key);
		if (state == GLFW_PRESS)
		{
			return key_action::press;
		}
		if (state == GLFW_RELEASE)
		{
			return key_action::release;
		}
	}

	void keyboard::key_callback(int which_key, int scancode, int action, int mods)
	{
		if (m_listener)
		{
			try
			{
				key k = glfw_key_to_enum(which_key);
				key_action k_action = key_action::press;

				switch (action)
				{
				case GLFW_PRESS:
				{
					k_action = key_action::press;
					break;
				}
				case GLFW_REPEAT:
				{
					k_action = key_action::repeat;
					break;
				}
				case GLFW_RELEASE:
				{
					k_action = key_action::release;
					break;
				}
				}

				uint16_t mod = 0;
				if (mods & GLFW_MOD_SHIFT)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::shift);
				}
				if (mods & GLFW_MOD_CONTROL)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::control);
				}
				if (mods & GLFW_MOD_ALT)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::alt);
				}
				if (mods & GLFW_MOD_SUPER)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::super);
				}
				if (mods & GLFW_MOD_CAPS_LOCK)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::caps_lock);
				}
				if (mods & GLFW_MOD_NUM_LOCK)
				{
					mod = mod | static_cast<decltype(mod)>(modifiers::num_lock);
				}

				modifiers converted_modifiers = static_cast<modifiers>(mod);

				m_listener->on_key_event(k, k_action, converted_modifiers);
			}
			catch (const std::exception& ex)
			{
				spdlog::error("Exception when handling key callback: {0}", ex.what());
			}
		}
	}

	void keyboard::char_callback(unsigned int codepoint)
	{
		if (m_listener)
		{
			m_listener->on_char_event(codepoint);
		}
	}

	int keyboard::convert_to_raw(key key)
	{
		return enum_to_glfw_key(key);
	}

	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyboard* kb = static_cast<user_pointer*>(glfwGetWindowUserPointer(window))->keyboard;
		kb->key_callback(key, scancode, action, mods);
	}

	void glfw_char_callback(GLFWwindow* window, unsigned int codepoint)
	{
		keyboard* kb = static_cast<user_pointer*>(glfwGetWindowUserPointer(window))->keyboard;
		kb->char_callback(codepoint);
	}
}