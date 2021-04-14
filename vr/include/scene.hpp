#pragma once

#include "object3d.hpp"

namespace vr
{
	class scene
	{
	public:
		void add(object3d* object); 
		void remove(object3d* object);

		const std::vector<object3d*>& get_objects() const;

	private:
		std::vector<object3d*> m_objects;
	};
}