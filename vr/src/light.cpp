#include "light.hpp"

namespace vr
{
	light::light(float intensity)
		: detail::has_intensity(intensity)
	{}
}