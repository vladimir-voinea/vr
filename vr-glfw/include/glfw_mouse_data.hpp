#pragma once

namespace vr::glfw
{
	struct mouse_position
	{
		double x;
		double y;
	};

	struct mouse_scroll
	{
		double xoffset;
		double yoffset;
	};
	
	enum class mouse_mode
	{
		normal,
		hidden,
		disabled
	};

	enum class mouse_action
	{
		press,
		release,
		repeat
	};

	enum class mouse_button
	{
		left,
		right,
		middle
	};
}