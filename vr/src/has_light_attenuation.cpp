#include "detail/has_light_attenuation.hpp"

namespace vr::detail
{
	has_light_attenuation::has_light_attenuation(const light_attenuation& attenuation)
		: m_attenuation(attenuation)
	{}

	has_light_attenuation::~has_light_attenuation() = default;

	const light_attenuation& has_light_attenuation::get_attenuation() const
	{
		return m_attenuation;
	}

	light_attenuation& has_light_attenuation::get_attenuation()
	{
		return m_attenuation;
	}

	void has_light_attenuation::set_attenuation(const light_attenuation& attenuation)
	{
		m_attenuation = attenuation;
	}
}