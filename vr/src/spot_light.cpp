#include "spot_light.hpp"

namespace vr
{
	spot_light::spot_light(const light_components& components, const light_attenuation& attenuation, float inner_cutoff_angle, float outer_cutoff_angle, float intensity)
		: point_light(components, attenuation, intensity)
		, m_inner_cutoff_angle(inner_cutoff_angle)
		, m_outer_cutoff_angle(outer_cutoff_angle)
	{}

	spot_light::~spot_light() = default;

	float spot_light::get_inner_cutoff_angle() const
	{
		return m_inner_cutoff_angle;
	}

	void spot_light::set_inner_cutoff_angle(float angle)
	{
		m_inner_cutoff_angle = angle;
	}

	float spot_light::get_outer_cutoff_angle() const
	{
		return m_outer_cutoff_angle;
	}

	void spot_light::set_outer_cutoff_angle(float angle)
	{
		m_outer_cutoff_angle = angle;
	}
}