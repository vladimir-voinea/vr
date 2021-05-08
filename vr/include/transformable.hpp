#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace vr
{
	class transformable
	{
	public:
		virtual ~transformable() = default;

		virtual glm::mat4 get_transformation_matrix() const;

		virtual const glm::vec3& get_scale() const;
		virtual void scale(const glm::vec3& scale);

		virtual glm::vec3 get_rotation() const;
		virtual void rotate(const glm::vec3& axis, float angle);
		virtual void rotate_world(const glm::vec3& axis, float angle);

		virtual const glm::vec3& get_translation() const;
		virtual void translate(const glm::vec3& position);

		virtual glm::vec3 front() const;
		virtual glm::vec3 right() const;
		virtual glm::vec3 up() const;

	protected:
		glm::vec3 m_position;
		glm::quat m_quaternion;
		glm::vec3 m_scale;
	};
}