#pragma once

#include "object3d.hpp"
#include "detail/has_intensity.hpp"

namespace vr
{
	class light : public object3d, public detail::has_intensity
	{
	public:
		light(float intensity);
		virtual ~light() override = 0;
	};
}