#include "camera.hpp"

namespace vr::gl
{
	glm::vec3 camera::get_position() const
	{
		return m_position;
	}

	void camera::set_position(const glm::vec3& position)
	{
		m_position = position;
	}

	glm::vec3 camera::get_direction() const
	{
		return m_direction;
	}

	void camera::set_direction(const glm::vec3& direction)
	{
		m_direction = direction;
	}
}