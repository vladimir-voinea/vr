#pragma once

#include <vr-glfw.hpp>

class controls
{
public:
	controls(vr::glfw_window& window);

private:
	vr::glfw_window& m_window;
};