#pragma once

#include "light_attenuation.hpp"

namespace vr::detail
{
	class has_light_attenuation
	{
	public:
		has_light_attenuation(const light_attenuation& attenuation);
		virtual ~has_light_attenuation();

		const light_attenuation& get_attenuation() const;
		light_attenuation& get_attenuation();

		void set_attenuation(const light_attenuation& attenuation);

	private:
		light_attenuation m_attenuation;
	};
}