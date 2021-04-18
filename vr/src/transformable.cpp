#include "transformable.hpp"

namespace vr
{
	glm::mat4 transformable::get_transformation_matrix() const
	{
		const auto translation = glm::translate(glm::mat4(1.f), m_position);
		const auto rotation = glm::toMat4(m_quaternion);

		return translation * rotation;
	}

	const glm::vec3& transformable::get_rotation() const
	{
		return glm::eulerAngles(m_quaternion);
	}

	void transformable::rotate(const glm::vec3& axis, float angle)
	{
		m_quaternion *= glm::quat(glm::angleAxis(angle, axis));
	}

	const glm::vec3& transformable::get_translation() const
	{
		return m_position;
	}

	void transformable::translate(const glm::vec3& position)
	{
		m_position += position;
	}
}