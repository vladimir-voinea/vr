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

		void scale(const glm::vec3& scale) override;

	protected:
		float m_zoom;
	};
}