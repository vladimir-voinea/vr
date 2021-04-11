#include "glfw_keyboard.hpp"

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <string>
#include <stdexcept>

namespace
{
	using k = vr::glfw::keyboard::key;
	const std::unordered_map<vr::glfw::keyboard::key, int> key_mapping = {
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
		{ k::escape, GLFW_KEY_ESCAPE }
	};

	int enum_to_glfw_key(const vr::glfw::keyboard::key& key)
	{
		auto it = key_mapping.find(key);
		if (it == key_mapping.end())
		{
			std::string message = "Keyboard key  enum value " +
				std::to_string(static_cast<uint16_t>(key)) + " has no mapping";
			throw std::runtime_error(message);
		}

		return it->second;
	}

	vr::glfw::keyboard::key char_to_enum(const char c)
	{
		if (c >= static_cast<char>(vr::glfw::keyboard::key::a) &&
			c <= static_cast<char>(vr::glfw::keyboard::key::z))
		{
			return static_cast<vr::glfw::keyboard::key>(c);
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
	}

	void keyboard::set_sticky_keys(bool value)
	{
		glfwSetInputMode(m_window.get_handle(), GLFW_STICKY_KEYS, value ? GL_TRUE : GL_FALSE);
	}

	keyboard::state keyboard::get_key_state(const keyboard::key& key)
	{
		const auto glfw_key = enum_to_glfw_key(key);
		const auto state = glfwGetKey(m_window.get_handle(), glfw_key);
		if (state == GLFW_PRESS)
		{
			return keyboard::state::press;
		}
		else if (state == GLFW_RELEASE)
		{
			return keyboard::state::release;
		}
	}
}