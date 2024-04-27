#pragma once

namespace vr::glfw
{
	class window;
	class keyboard;
	class mouse;

	struct user_pointer
	{
		window* window_ptr;
		keyboard* keyboard_ptr;
		mouse* mouse_ptr;
	};
}