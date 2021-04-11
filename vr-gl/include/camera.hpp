#pragma once

#include <glm/glm.hpp>

namespace vr::gl
{
	class camera
	{
	public:
		virtual ~camera() = default;
		virtual glm::mat4 get_projection_matrix() const = 0;

		glm::vec3 get_position() const;
		void set_position(const glm::vec3& position);

		glm::vec3 get_direction() const;
		void set_direction(const glm::vec3& position);
		
	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
	};
}