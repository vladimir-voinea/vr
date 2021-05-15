#pragma once

#include "point_light.hpp"

namespace vr
{
	class spot_light : public point_light
	{
	public:
		spot_light(const light_components& components, const light_attenuation& attenuation, float inner_cutoff_angle, float outer_cutoff_angle, float intensity = 1.f);
		~spot_light() override;

		float get_inner_cutoff_angle() const;
		void set_inner_cutoff_angle(float angle);

		float get_outer_cutoff_angle() const;
		void set_outer_cutoff_angle(float angle);

		void load_uniforms(const uniform_loader_base& loader, unsigned int light_index) override;

	private:
		float m_inner_cutoff_angle;
		float m_outer_cutoff_angle;
	};
}