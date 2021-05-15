#pragma once

#include "light.hpp"
#include "detail/has_light_components.hpp"
#include "detail/has_light_attenuation.hpp"

namespace vr
{
	class point_light : public light, public detail::has_light_components, public detail::has_light_attenuation
	{
	public:
		point_light(const light_components& components, const light_attenuation& attenuation, float intensity = 1.f);
		virtual ~point_light() override;

		void load_uniforms(const uniform_loader_base& loader, unsigned int light_index) override;

	private:
	};
}