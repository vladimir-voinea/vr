#include "transformable.hpp"

#include "defs.hpp"

namespace vr
{
	glm::mat4 transformable::get_transformation_matrix() const
	{
		const auto translation = glm::translate(glm::mat4(1.f), m_position);
		const auto rotation = glm::toMat4(m_quaternion);
		const auto scale = glm::scale(glm::mat4(1.f), m_scale);

		return translation * rotation;// *scale;
	}

	const glm::vec3& transformable::get_scale() const
	{
		return m_position;
	}

	void transformable::scale(const glm::vec3& scale)
	{
		m_scale += scale;
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

	const glm::vec3& transformable::get_translation() const
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