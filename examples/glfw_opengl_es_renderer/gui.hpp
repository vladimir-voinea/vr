#pragma once

#include <glfw_window.hpp>

#include <string>

class gui
{
public:
	gui(vr::glfw::window& window);

	void frame();

public:

	float f = 0.5f;
	std::string text_input = "Type here";
};