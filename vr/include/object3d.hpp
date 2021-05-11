#pragma once

#include "mesh.hpp"
#include "shader_material.hpp"
#include "texture.hpp"
#include "transformable.hpp"

#include <glm/glm.hpp>

#include <utility>
#include <functional>
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

		const std::vector<mesh*>& get_meshes() const;
		std::vector<mesh*>& get_meshes();
		void add_mesh(mesh*);
		void remove_mesh(const mesh*);

		bool has_geometry() const;

		virtual glm::mat4 get_transformation_matrix() const override;

		const std::string& get_name() const;
		void set_name(const std::string& name);

		void set_before_render_callback(std::function<void(void)> callback);
		void on_before_render();

		void traverse(std::function<void(object3d*)> func);

	private:
		void set_parent(object3d*);

	private:
		object3d* m_parent = nullptr;
		std::vector<std::unique_ptr<object3d>> m_children;
		std::vector<mesh*> m_meshes;
		glm::vec3 m_position;
		glm::vec3 m_rotation;

		std::string m_name;

		std::function<void(void)> m_before_render_callback;
	};
}