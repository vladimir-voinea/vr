#pragma once

#include "mesh.hpp"
#include "shader_material.hpp"
#include "texture.hpp"
#include "renderer_object.hpp"

#include <glm/glm.hpp>

#include <utility>

namespace vr
{
	class object3d : public has_renderer_object
	{
	public:
		object3d();

		object3d* get_parent();
		const object3d* get_parent() const;
		void set_parent(object3d*);

		const std::vector<object3d*>& get_children() const;
		void add_child(object3d*);
		void remove_child(object3d*);

		const std::vector<const mesh*>& get_meshes() const;
		void add_mesh(const mesh*);
		void remove_mesh(const mesh*);

		bool has_geometry() const;

		virtual glm::mat4 get_transformation_matrix() const;

		void set_scale();

		const glm::vec3& get_rotation() const;
		void set_rotation(const glm::vec3& rotation);

		const glm::vec3& get_position() const;
		void set_position(const glm::vec3& position);

	private:
		object3d* m_parent = nullptr;
		std::vector<object3d*> m_children;
		std::vector<const mesh*> m_meshes;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
	};
}