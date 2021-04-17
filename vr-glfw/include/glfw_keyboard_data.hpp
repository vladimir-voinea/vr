#pragma once

#include <stdint.h>

namespace vr::glfw
{
	enum class key_action
	{
		press,
		release,
		repeat
	};

	enum class modifiers : uint16_t
	{
		shift = 1 << 1,
		control = 1 << 2,
		alt = 1 << 3,
		super = 1 << 4,
		caps_lock = 1 << 5,
		num_lock = 1 << 6
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
}