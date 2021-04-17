#pragma once

namespace vr::glfw
{
	struct mouse_position
	{
		double x;
		double y;
	};

	enum class mouse_mode
	{
		normal,
		hidden,
		disabled
	};
}