#pragma once

#include <string>

namespace vr
{
	struct window_settings
	{
		int width = 800;
		int height = 600;
		std::string name = "Unnamed window";
		bool create_opengl_context = false;
		bool resizable = false;
	};
}