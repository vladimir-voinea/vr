#pragma once

#include "object3d.hpp"

#include "ambient_light.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"

#include <vector>

namespace vr
{
	class scene : public object3d
	{
	public:
		scene();
		~scene() override;

		void add(const ambient_light* light);
		void add(const directional_light* light);
		void add(const point_light* light);
		void add(const spot_light* light);

		void remove(const ambient_light* light);
		void remove(const directional_light* light);
		void remove(const point_light* light);
		void remove(const spot_light* light);

	private:
		const ambient_light* m_ambient_light = nullptr;
		const directional_light* m_directional_light = nullptr;
		std::vector<const point_light*> m_point_lights;
		std::vector<const spot_light*> m_spot_lights;
	};
}