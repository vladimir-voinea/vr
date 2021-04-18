#include "camera.hpp"

namespace vr
{
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;

	camera::camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float yaw, float pitch)
		: m_position(position)
		, m_front(direction)
		, m_world_up(up)
		, m_pitch(pitch)
		, m_yaw(yaw)
		, m_movement_speed(SPEED)
		, m_mouse_sensitivity(SENSITIVITY)
		, m_zoom(ZOOM)
	{
		update_camera_vectors();
	}

	camera::~camera() = default;
	
	glm::mat4 camera::get_view_matrix() const
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void camera::process_keyboard(Camera_Movement direction, float delta_time)
	{
		float velocity = m_movement_speed * delta_time;
		if (direction == Camera_Movement::FORWARD)
			m_position += m_front * velocity;
		if (direction == Camera_Movement::BACKWARD)
			m_position -= m_front * velocity;
		if (direction == Camera_Movement::LEFT)
			m_position -= m_right * velocity;
		if (direction == Camera_Movement::RIGHT)
			m_position += m_right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void camera::process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch)
	{
		xoffset *= m_mouse_sensitivity;
		yoffset *= m_mouse_sensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

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
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
}