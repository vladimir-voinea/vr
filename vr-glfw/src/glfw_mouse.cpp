#include "glfw_mouse.hpp"

#include "GLFW/glfw3.h"

namespace vr::glfw
{
	mouse::mouse(window& window)
		: m_window(window)
	{
	}

	mouse::position mouse::get_position()
	{
		mouse::position position;
		glfwGetCursorPos(m_window.get_handle(), &position.x, &position.y);

		return position;
	}

	void mouse::set_position(const mouse::position& position)
	{
		glfwSetCursorPos(m_window.get_handle() , position.x, position.y);
	}
}