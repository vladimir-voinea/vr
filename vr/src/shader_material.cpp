#include "shader_material.hpp"

namespace vr
{
	shader_material::shader_material() = default;
	shader_material::~shader_material() = default;


	const std::string& shader_material::get_name() const
	{
		return m_name;
	}

	void shader_material::set_name(const std::string& name)
	{
		m_name = name;
	}

	const shading_model& shader_material::get_shading_model() const
	{
		return m_shading_model;
	}

	void shader_material::set_shading_model(const shading_model& model)
	{
		m_shading_model = model;
	}

	const glm::vec3& shader_material::get_ambient() const
	{
		return m_ambient;
	}

	void shader_material::set_ambient(const glm::vec3& ambient)
	{
		m_ambient = ambient;
	}

	const glm::vec3& shader_material::get_diffuse() const
	{
		return m_diffuse;
	}

	void shader_material::set_diffuse(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
	}

	const glm::vec3& shader_material::get_specular() const
	{
		return m_specular;
	}

	void shader_material::set_specular(const glm::vec3& specular)
	{
		m_specular = specular;
	}

	const float& shader_material::get_shininess() const
	{
		return m_shininess;
	}

	void shader_material::set_shininess(float shininess)
	{
		m_shininess = shininess;
	}
}