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
		void set_parent(object3d*);

		const std::vector<object3d*>& get_children() const;
		void add_child(object3d*);
		void remove_child(object3d*);

		const std::vector<const mesh*>& get_meshes() const;
		void add_mesh(const mesh*);
		void remove_mesh(const mesh*);

		bool has_geometry() const;

	private:
		object3d* m_parent = nullptr;
		std::vector<object3d*> m_children;

		std::vector<const mesh*> m_meshes;
	};
}