#include "glfw_mouse.hpp"
#include "glfw_user_pointer.hpp"

#include "GLFW/glfw3.h"

namespace vr::glfw
{
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	void glfw_button_callback(GLFWwindow* window, int button, int action, int mods);
	void glfw_button_callback(GLFWwindow* window, int button, int action, int mods);
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void glfw_cursor_enter_callback(GLFWwindow* window, int entered);

	mouse::mouse(window& window)
		: m_window(window)
	{
		static_cast<user_pointer*>(glfwGetWindowUserPointer(window.get_handle()))->mouse_ptr = this;
		glfwSetCursorPosCallback(window.get_handle(), glfw_cursor_position_callback);
		glfwSetMouseButtonCallback(window.get_handle(), glfw_button_callback);
		glfwSetScrollCallback(window.get_handle(), glfw_scroll_callback);
		glfwSetCursorEnterCallback(window.get_handle(), glfw_cursor_enter_callback);
	}

	void mouse::set_listener(mouse_listener* listener)
	{
		m_listener = listener;
	}

	void mouse::set_sticky_buttons(bool value)
	{
		glfwSetInputMode(m_window.get_handle(),
			GLFW_STICKY_MOUSE_BUTTONS, value ? GLFW_TRUE : GLFW_FALSE);
	}

	void mouse::set_raw_motion(bool value)
	{
		if (glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(m_window.get_handle(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}

	mouse_position mouse::get_position()
	{
		mouse_position position;
		glfwGetCursorPos(m_window.get_handle(), &position.x, &position.y);

		return position;
	}

	void mouse::set_position(const mouse_position& position)
	{
		glfwSetCursorPos(m_window.get_handle(), position.x, position.y);
	}

	void mouse::set_mode(const mouse_mode& mode)
	{
		int glfw_mode = 0;
		switch (mode)
		{
		case mouse_mode::normal:
		{
			glfw_mode = GLFW_CURSOR_NORMAL;
			break;
		}
		case mouse_mode::hidden:
		{
			glfw_mode = GLFW_CURSOR_HIDDEN;
			break;
		}
		case mouse_mode::disabled:
		{
			glfw_mode = GLFW_CURSOR_DISABLED;
			break;
		}
		}

		glfwSetInputMode(m_window.get_handle(), GLFW_CURSOR, glfw_mode);
	}

	void mouse::cursor_position_callback(double xpos, double ypos)
	{
		if (m_listener)
		{
			m_listener->on_position_event({ xpos, ypos });
		}
	}

	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		auto user_ptr = static_cast<user_pointer*>(glfwGetWindowUserPointer(window));
		auto m = static_cast<mouse*>(user_ptr->mouse_ptr);
		m->cursor_position_callback(xpos, ypos);
	}

	void mouse::button_callback(int button, int action, int mods)
	{
		if (m_listener)
		{
			mouse_button b = mouse_button::left;
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				b = mouse_button::left;
				break;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				b = mouse_button::right;
				break;
			}
			case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				b = mouse_button::middle;
				break;
			}
			}

			mouse_action a = mouse_action::press;
			switch (action)
			{
			case GLFW_PRESS:
			{
				a = mouse_action::press;
				break;
			}
			case GLFW_RELEASE:
			{
				a = mouse_action::release;
				break;
			}
			case GLFW_REPEAT:
			{
				a = mouse_action::repeat;
				break;
			}
			}

			m_listener->on_button_event(b, a);
		}
	}

	void glfw_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		auto user_ptr = static_cast<user_pointer*>(glfwGetWindowUserPointer(window));
		auto m = static_cast<mouse*>(user_ptr->mouse_ptr);
		m->button_callback(button, action, mods);
	}

	void mouse::scroll_callback(double xoffset, double yoffset)
	{
		if (m_listener)
		{
			m_listener->on_scroll_event({ xoffset, yoffset });
		}
	}

	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto user_ptr = static_cast<user_pointer*>(glfwGetWindowUserPointer(window));
		auto m = static_cast<mouse*>(user_ptr->mouse_ptr);
		m->scroll_callback(xoffset, yoffset);
	}

	void mouse::cursor_enter_callback(int entered)
	{
		if (m_listener)
		{
			m_listener->on_enter_event(entered);
		}
	}

	void glfw_cursor_enter_callback(GLFWwindow* window, int entered)
	{
		auto user_ptr = static_cast<user_pointer*>(glfwGetWindowUserPointer(window));
		auto m = static_cast<mouse*>(user_ptr->mouse_ptr);
		m->cursor_enter_callback(entered);
	}
}