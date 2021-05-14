#include "point_light.hpp"

namespace vr
{
	point_light::point_light(const light_components& components, const light_attenuation& attenuation, float intensity)
		: light(intensity)
		, detail::has_light_components(components)
		, detail::has_light_attenuation(attenuation)
	{}

	point_light::~point_light() = default;
}