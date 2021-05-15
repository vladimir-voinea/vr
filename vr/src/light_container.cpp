#include "light_container.hpp"

namespace vr
{
	void light_container::add(ambient_light* light)
	{
		m_ambient_light = light;
	}

	void light_container::add(directional_light* light)
	{
		m_directional_light = light;
	}

	void light_container::add(point_light* light)
	{
		m_point_lights.push_back(light);
	}

	void light_container::add(spot_light* light)
	{
		m_spot_lights.push_back(light);
	}


	void light_container::remove(ambient_light* light)
	{
		m_ambient_light = nullptr;
	}

	void light_container::remove(directional_light* light)
	{
		m_directional_light = nullptr;
	}

	void light_container::remove(point_light* light)
	{
		m_point_lights.erase(std::remove(m_point_lights.begin(), m_point_lights.end(), light));
	}

	void light_container::remove(spot_light* light)
	{
		m_spot_lights.erase(std::remove(m_spot_lights.begin(), m_spot_lights.end(), light));
	}

	void light_container::load_uniforms(const uniform_loader_base& loader)
	{
		loader.load_uniform(make_uniform("vr_have_ambient_light", m_ambient_light != nullptr));
		if (m_ambient_light)
		{
			m_ambient_light->load_uniforms(loader, 0);
		}

		loader.load_uniform(make_uniform("vr_have_directional_light", m_directional_light != nullptr));
		if (m_directional_light)
		{
			m_directional_light->load_uniforms(loader, 0);
		}

		loader.load_uniform(make_uniform("vr_n_point_lights", static_cast<int>(m_point_lights.size())));
		for (auto i = 0u; i < m_point_lights.size(); ++i)
		{
			m_point_lights[i]->load_uniforms(loader, i);
		}

		loader.load_uniform(make_uniform("vr_have_spot_light", !m_spot_lights.empty()));
		for (auto i = 0u; i < std::min(static_cast<std::size_t>(1), m_spot_lights.size()); ++i)
		{
			m_spot_lights[i]->load_uniforms(loader, i);
		}
	}
}