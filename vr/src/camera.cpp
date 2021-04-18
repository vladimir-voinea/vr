#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <spdlog/spdlog.h>

namespace vr
{
	const float ZOOM = 45.0f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;

	camera::camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch, float zoom)
		: m_position(position)
		, m_front(direction)
		, m_world_up(up)
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_zoom(zoom)
	{
		update_camera_vectors();
	}

	camera::~camera() = default;
	
	glm::mat4 camera::get_view_matrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void camera::translate(const glm::vec3& translation)
	{
		m_position += translation;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void camera::process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch)
	{
		m_yaw += xoffset;
		m_pitch += yoffset;

		spdlog::info("Pitch: {0}, Yaw: {1} after mouse event", m_pitch, m_yaw);

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		update_camera_vectors();
	}

	void camera::rotate(const glm::vec3& axis, float angle)
	{
		update_front_from_angle_axis(axis, angle);
		update_yaw_and_pitch_from_front();
		update_right_and_up_vectors();

		spdlog::info("Pitch: {0}, Yaw: {1} after rotate event", m_pitch, m_yaw);
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void camera::process_mouse_scroll(float yoffset)
	{
		m_zoom -= (float)yoffset;
		if (m_zoom < 1.0f)
			m_zoom = 1.0f;
		if (m_zoom > 45.0f)
			m_zoom = 45.0f;
	}

	// calculates the front vector from the Camera's (updated) Euler Angles
	void camera::update_camera_vectors()
	{
		// calculate the new Front vector
		update_front_from_yaw_and_pitch();

		// also re-calculate the Right and Up vector
		update_right_and_up_vectors();
	}

	void camera::update_front_from_angle_axis(const glm::vec3& axis, float angle)
	{
		const auto rot = glm::rotate(glm::mat4(1.f), angle, axis);
		const auto new_front4 = rot * glm::vec4(m_front.x, m_front.y, m_front.z, 0.0);
		m_front = glm::normalize(glm::vec3(new_front4.x, new_front4.y, new_front4.z));
	}

	void camera::update_front_from_yaw_and_pitch()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_front = glm::normalize(front);
	}

	void camera::update_right_and_up_vectors()
	{
		m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

	void camera::update_yaw_and_pitch_from_front()
	{
		m_pitch = glm::degrees(std::asin(-m_front.y));
		m_yaw = glm::degrees(std::atan2(m_front.z, m_front.x));
	}
}