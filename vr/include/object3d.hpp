#pragma once

#include "mesh.hpp"
#include "shader_material.hpp"
#include "texture.hpp"
#include "transformable.hpp"

#include <glm/glm.hpp>

#include <utility>
#include <memory>

namespace vr
{
	class object3d : public transformable
	{
	public:
		object3d();
		~object3d() override = default;

		object3d* get_parent();
		const object3d* get_parent() const;

		const std::vector<std::unique_ptr<object3d>>& get_children() const;
		void add_child(std::unique_ptr<object3d>);
		void remove_child(object3d*);

		const std::vector<const mesh*>& get_meshes() const;
		void add_mesh(const mesh*);
		void remove_mesh(const mesh*);

		bool has_geometry() const;

		virtual glm::mat4 get_transformation_matrix() const override;

	private:
		void set_parent(object3d*);

	private:
		object3d* m_parent = nullptr;
		std::vector<std::unique_ptr<object3d>> m_children;
		std::vector<const mesh*> m_meshes;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
	};
}