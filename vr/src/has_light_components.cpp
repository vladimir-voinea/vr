#include "detail/has_light_components.hpp"

namespace vr::detail
{
	has_light_components::has_light_components(const light_components& components)
		: m_components(components)
	{}

	has_light_components::~has_light_components() = default;

	const light_components& has_light_components::get_components() const
	{
		return m_components;
	}

	light_components& has_light_components::get_components()
	{
		return m_components;
	}

	void has_light_components::set_components(const light_components& components)
	{
		m_components = components;
	}
}