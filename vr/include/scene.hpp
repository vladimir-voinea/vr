#pragma once

#include "object3d.hpp"

#include "light_container.hpp"

#include <vector>

namespace vr
{
	class scene : public object3d
	{
	public:
		scene();
		~scene() override;

		const light_container& get_lights() const;
		light_container& get_lights();

	private:
		light_container m_lights;
	};
}