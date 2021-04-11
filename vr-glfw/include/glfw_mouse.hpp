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

		enum class mode
		{
			normal,
			hidden,
			disabled
		};

	public:
		mouse(window& window);
		mouse(const mouse&) = delete;
		mouse& operator=(const mouse&) = delete;

		void set_sticky_buttons(bool value);

		position get_position();
		void set_position(const position& position);

		void set_mode(const mode& mode);

	private:
		window& m_window;
	};
}