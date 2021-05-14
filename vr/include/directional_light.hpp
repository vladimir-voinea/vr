#pragma once

#include "light.hpp"
#include "detail/has_light_components.hpp"

namespace vr
{
	class directional_light : public light, public vr::detail::has_light_components
	{
	public:
		directional_light(const light_components& components, float intensity = 1.f);
		~directional_light() override;

	private:
		float m_intensity;
	};
}