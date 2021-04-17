#pragma once

#include <glm/glm.hpp>

namespace vr
{
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum class Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class camera
	{
	public:
		virtual ~camera() = default;
		virtual glm::mat4 get_projection_matrix() const = 0;
		virtual glm::mat4 get_view_matrix() const = 0;

		virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
		virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;
		virtual void ProcessMouseScroll(float yoffset) = 0;
		
	private:
	};
}