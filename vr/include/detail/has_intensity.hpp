#pragma once

namespace vr::detail
{
	class has_intensity
	{
	public:
		has_intensity(float intensity = 1.f);
		virtual ~has_intensity();

		const float& get_intensity() const;
		float& get_intensity();

		void set_intensity(float value);

	private:
		float m_intensity;
	};
}