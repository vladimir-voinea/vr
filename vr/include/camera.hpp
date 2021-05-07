#pragma once

#include <object3d.hpp>
#include <glm/glm.hpp>

namespace vr
{
	class camera : public object3d
	{
	public:
		camera(glm::vec3 position, glm::vec3 direction, float zoom);
		virtual ~camera() override = default;

		virtual glm::mat4 get_projection_matrix() const = 0;
		virtual glm::mat4 get_view_matrix() const;

		//void process_mouse_movement(float xoffset, float yoffset, bool constrain_pitch = true);
		void process_mouse_scroll(float yoffset);

	protected:
		float m_zoom;
	};
}