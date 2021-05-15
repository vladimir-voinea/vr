#include "scene.hpp"

namespace vr
{
	scene::scene()
		: object3d()
	{}

	scene::~scene() = default;


	const light_container& scene::get_lights() const
	{
		return m_lights;
	}

	light_container& scene::get_lights()
	{
		return m_lights;
	}

}