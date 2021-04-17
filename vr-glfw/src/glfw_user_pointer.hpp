#pragma once

namespace vr::glfw
{
	class window;
	class keyboard;
	class mouse;

	struct user_pointer
	{
		window* window;
		keyboard* keyboard;
		mouse* mouse;
	};
}