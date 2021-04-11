#pragma once

#include "glfw_window.hpp"

namespace vr::glfw
{
	class keyboard
	{
	public:
		enum class state
		{
			press,
			release
		};

		enum class key : uint16_t
		{
			a = 97,
			b,
			c,
			d,
			e,
			f,
			g,
			h,
			i,
			j,
			k,
			l,
			m,
			n,
			o,
			p,
			q,
			r,
			s,
			t,
			u,
			v,
			w,
			x,
			y,
			z,
			space,
			left_ctrl,
			left_shift,
			escape
		};

	public:
		keyboard(window& window);

		state get_key_state(const key& key);

	private:
		window& m_window;
	};
}