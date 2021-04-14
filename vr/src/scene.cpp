#include "scene.hpp"

namespace vr
{
	void scene::add(object3d* object)
	{
		m_objects.push_back(object);
	}

	void scene::remove(object3d* object)
	{
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object));
	}

	const std::vector<object3d*>& scene::get_objects() const
	{
		return m_objects;
	}
}