#pragma once

#include "light_components.hpp"

namespace vr::detail
{
	class has_light_components
	{
	public:
		has_light_components(const light_components& components);
		virtual ~has_light_components();

		const light_components& get_components() const;
		light_components& get_components();

		void set_components(const light_components& components);

	private:
		light_components m_components;
	};
}