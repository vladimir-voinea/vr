#include "glfw_mouse.hpp"

#include "GLFW/glfw3.h"

namespace vr::glfw
{
	mouse::mouse(window& window)
		: m_window(window)
	{
	}

	void mouse::set_sticky_buttons(bool value)
	{
		glfwSetInputMode(m_window.get_handle(),
			GLFW_STICKY_MOUSE_BUTTONS, value ? GLFW_TRUE : GLFW_FALSE);
	}

	mouse::position mouse::get_position()
	{
		mouse::position position;
		glfwGetCursorPos(m_window.get_handle(), &position.x, &position.y);

		return position;
	}

	void mouse::set_position(const mouse::position& position)
	{
		glfwSetCursorPos(m_window.get_handle(), position.x, position.y);
	}

	void mouse::set_mode(const mouse::mode& mode)
	{
		int glfw_mode = 0;
		switch (mode)
		{
		case mouse::mode::normal:
		{
			glfw_mode = GLFW_CURSOR_NORMAL;
			break;
		}
		case mouse::mode::hidden:
		{
			glfw_mode = GLFW_CURSOR_HIDDEN;
			break;
		}
		case mouse::mode::disabled:
		{
			glfw_mode = GLFW_CURSOR_DISABLED;
			break;
		}
		}

		glfwSetInputMode(m_window.get_handle(), GLFW_CURSOR, glfw_mode);
	}
}