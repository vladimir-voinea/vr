#pragma once

#include "object3d.hpp"
#include "detail/has_intensity.hpp"

#include "uniform_loader_base.hpp"

namespace vr
{
	class light : public object3d, public detail::has_intensity
	{
	public:
		light(float intensity);
		virtual ~light() override;

		virtual void load_uniforms(const uniform_loader_base& loader, unsigned int light_index) = 0;
	};
}