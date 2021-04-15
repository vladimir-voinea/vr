#include "object3d.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

	const std::vector<object3d*>& object3d::get_children() const
	{
		return m_children;
	}

	void object3d::add_child(object3d* child)
	{
		m_children.push_back(child);
	}

	void object3d::remove_child(object3d* child)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
	}

	const std::vector<const mesh*>& object3d::get_meshes() const
	{
		return m_meshes;
	}

	void object3d::add_mesh(const mesh* mesh)
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
}
