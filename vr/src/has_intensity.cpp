#include "detail/has_intensity.hpp"

namespace vr::detail
{
	has_intensity::has_intensity(float intensity)
		: m_intensity(intensity)
	{}

	has_intensity::~has_intensity() = default;

	const float& has_intensity::get_intensity() const
	{
		return m_intensity;
	}

	float& has_intensity::get_intensity()
	{
		return m_intensity;
	}

	void has_intensity::set_intensity(float value)
	{
		m_intensity = value;
	}
}