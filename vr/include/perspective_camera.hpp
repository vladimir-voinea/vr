#pragma once

#include "camera.hpp"
#include <glm/glm.hpp>

namespace vr
{
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class perspective_camera : public camera
	{
	public:
		struct settings
		{
			float fov;
			float aspect_ratio;
			float near;
			float far;
			
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
			glm::vec3 up = glm::vec3(0.f, 0.1f, 0.f);
			float pitch = 0.f;
			float yaw = -90.f;
		};

	public:
		perspective_camera(const settings& settings);

		glm::mat4 get_projection_matrix() const override;
		glm::mat4 get_view_matrix() const override;

		// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(Camera_Movement direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == Camera_Movement::FORWARD)
				Position += Front * velocity;
			if (direction == Camera_Movement::BACKWARD)
				Position -= Front * velocity;
			if (direction == Camera_Movement::LEFT)
				Position -= Right * velocity;
			if (direction == Camera_Movement::RIGHT)
				Position += Right * velocity;
		}

		// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			// update Front, Right and Up Vectors using the updated Euler angles
			updateCameraVectors();
		}

		// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset)
		{
			Zoom -= (float)yoffset;
			if (Zoom < 1.0f)
				Zoom = 1.0f;
			if (Zoom > 45.0f)
				Zoom = 45.0f;
		}

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors()
		{
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			// also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}

	private:
		const settings& m_settings;
		// camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		// camera options
		float MovementSpeed = SPEED;
		float MouseSensitivity = SENSITIVITY;
		float Zoom = ZOOM;
};
}