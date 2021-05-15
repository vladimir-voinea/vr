#pragma once

#include "ambient_light.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"

#include "uniform_loader_base.hpp"

#include <vector>

namespace vr
{
	class light_container
	{
	public:
		void add(ambient_light* light);
		void add(directional_light* light);
		void add(point_light* light);
		void add(spot_light* light);

		void remove(ambient_light* light);
		void remove(directional_light* light);
		void remove(point_light* light);
		void remove(spot_light* light);

		void load_uniforms(const uniform_loader_base& loader);

	private:
		ambient_light* m_ambient_light = nullptr;
		directional_light* m_directional_light = nullptr;
		std::vector<point_light*> m_point_lights;
		std::vector<spot_light*> m_spot_lights;
	};
}