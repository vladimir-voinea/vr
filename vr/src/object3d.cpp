#include "object3d.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <spdlog/spdlog.h>

namespace vr
{
	object3d::object3d() = default;

	object3d* object3d::get_parent()
	{
		return m_parent;
	}

	const object3d* object3d::get_parent() const
	{
		return m_parent;
	}

	void object3d::set_parent(object3d* parent)
	{
		m_parent = parent;
	}

	const std::vector<std::unique_ptr<object3d>>& object3d::get_children() const
	{
		return m_children;
	}

	void object3d::add_child(std::unique_ptr<object3d> child)
	{
		m_children.emplace_back(std::move(child))->set_parent(this);
	}

	std::unique_ptr<object3d> object3d::remove_child(object3d* child)
	{
		auto remove_it = std::remove_if(m_children.begin(), m_children.end(), [child](const auto& child_ptr) { return child_ptr.get() == child; });

		std::unique_ptr<object3d> released = std::move(*remove_it);
		m_children.erase(remove_it, m_children.end());
		released->set_parent(nullptr);

		return released;
	}

	const std::vector<mesh*>& object3d::get_meshes() const
	{
		return m_meshes;
	}

	std::vector<mesh*>& object3d::get_meshes()
	{
		return m_meshes;
	}

	void object3d::add_mesh(mesh* mesh)
	{
		m_meshes.push_back(mesh);
	}

	void object3d::remove_mesh(const mesh* mesh)
	{
		m_meshes.erase(std::remove(m_meshes.begin(), m_meshes.end(), mesh), m_meshes.end());
	}

	bool object3d::has_geometry() const
	{
		return !m_meshes.empty();
	}

	glm::mat4 object3d::get_transformation_matrix() const
	{
		if (get_parent() != nullptr)
		{
			return get_parent()->get_transformation_matrix() * transformable::get_transformation_matrix();
		}
		else
		{
			return transformable::get_transformation_matrix();
		}
	}

	const std::string& object3d::get_name() const
	{
		return m_name;
	}

	void object3d::set_name(const std::string& name)
	{
		m_name = name;
	}

	void object3d::set_before_render_callback(std::function<void(void)> callback)
	{
		m_before_render_callback = callback;
	}

	void object3d::on_before_render()
	{
		if (m_before_render_callback)
		{
			m_before_render_callback();
		}
	}

	void object3d::traverse(std::function<void(object3d*)> func)
	{
		func(this);

		for (auto& child : get_children())
		{
			child->traverse(func);
		}
	}
}
