#pragma once

#include <glm/glm.hpp>

namespace vr
{
	class camera
	{
	public:
		virtual ~camera() = default;
		virtual const glm::mat4& get_projection_matrix() const = 0;
		
		const glm::mat4& get_view_matrix() const;
		void set_view_matrix(const glm::mat4& view_matrix);

		glm::vec3 get_position() const;
		void set_position(const glm::vec3& position);

		glm::vec3 get_direction() const;
		void set_direction(const glm::vec3& position);
		
	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
		glm::mat4 m_view_matrix;
	};
}