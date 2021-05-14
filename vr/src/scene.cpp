#include "scene.hpp"

namespace vr
{
	scene::scene()
		: object3d()
	{}

	scene::~scene() = default;

	void scene::add(const ambient_light* light)
	{
		m_ambient_light = light;
	}

	void scene::add(const directional_light* light)
	{
		m_directional_light = light;
	}

	void scene::add(const point_light* light)
	{
		m_point_lights.push_back(light);
	}

	void scene::add(const spot_light* light)
	{
		m_spot_lights.push_back(light);
	}


	void scene::remove(const ambient_light* light)
	{
		m_ambient_light = nullptr;
	}

	void scene::remove(const directional_light* light)
	{
		m_directional_light = nullptr;
	}

	void scene::remove(const point_light* light)
	{
		m_point_lights.erase(std::remove(m_point_lights.begin(), m_point_lights.end(), light));
	}

	void scene::remove(const spot_light* light)
	{
		m_spot_lights.erase(std::remove(m_spot_lights.begin(), m_spot_lights.end(), light));
	}

}