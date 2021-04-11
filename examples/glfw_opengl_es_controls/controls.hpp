#pragma once

#include <vr-glfw.hpp>

class controls
{
public:
	controls(vr::glfw::window& window);

private:
	vr::glfw::window& m_window;
};