#include "transformable.hpp"

#include "defs.hpp"

namespace vr
{
	glm::mat4 transformable::get_transformation_matrix() const
	{
		const auto translation = glm::translate(glm::mat4(1.f), m_position);
		const auto rotation = glm::toMat4(m_quaternion);

		return translation * rotation;
	}

	glm::vec3 transformable::get_rotation() const
	{
		return glm::eulerAngles(m_quaternion);
	}

	void transformable::rotate(const glm::vec3& axis, float angle)
	{
		m_quaternion *= glm::quat(glm::angleAxis(glm::radians(angle), axis));
	}

	void transformable::rotate_world(const glm::vec3& axis, float angle)
	{
		m_quaternion = glm::quat(glm::angleAxis(glm::radians(angle), axis)) * m_quaternion;
	}

	glm::vec3 transformable::get_translation() const
	{
		return m_position;
	}

	void transformable::translate(const glm::vec3& position)
	{
		m_position += position;
	}

	glm::vec3 transformable::front() const
	{
		return glm::rotate(m_quaternion, -vr::z_axis);
	}

	glm::vec3 transformable::right() const
	{
		return glm::rotate(m_quaternion, vr::x_axis);
	}

	glm::vec3 transformable::up() const
	{
		return glm::rotate(m_quaternion, vr::y_axis);
	}
}