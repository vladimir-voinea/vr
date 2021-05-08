#pragma once

#include <glm/glm.hpp>

#include <string>

namespace vr
{
	enum class shading_model
	{
		phong,
		gourad
	};

	class shader_material
	{
	public:
		shader_material();
		virtual ~shader_material();

		const std::string& get_name() const;
		void set_name(const std::string& name);

		const shading_model& get_shading_model() const;
		void set_shading_model(const shading_model& model);

		const glm::vec3& get_ambient() const;
		void set_ambient(const glm::vec3& ambient);

		const glm::vec3& get_diffuse() const;
		void set_diffuse(const glm::vec3& diffuse);

		const glm::vec3& get_specular() const;
		void set_specular(const glm::vec3& specular);

		const float& get_shininess() const;
		void set_shininess(float shininess);

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;

		std::string m_name;
		shading_model m_shading_model;
	};
}