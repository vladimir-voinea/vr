#pragma once

#include "glfw_window.hpp"

namespace vr::glfw
{
	class mouse
	{
	public:
		struct position
		{
			double x;
			double y;
		};

	public:
		mouse(window& window);
		mouse(const mouse&) = delete;
		mouse& operator=(const mouse&) = delete;

		position get_position();
		void set_position(const position& position);

	private:
		window& m_window;
	};
}