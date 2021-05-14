#include "directional_light.hpp"

namespace vr
{
	directional_light::directional_light(const light_components& components, float intensity)
		: light(intensity)
		, detail::has_light_components(components)
		, m_intensity(intensity)
	{}

	directional_light::~directional_light() = default;
}