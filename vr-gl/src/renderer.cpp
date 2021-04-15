#include "renderer.hpp"
#include "opengl_shader_material.hpp"
#include "gpu_objects.hpp"
#include "renderer_cache.hpp"
#include "texture_loader.hpp"
#include <shader_material.hpp>

#include <map>
#include <iostream>

namespace
{
	constexpr auto builtin_vertex_position_attribute_name = "vr_vertex_position";
	constexpr auto builtin_vertex_normal_attribute_name = "vr_vertex_normal";
	constexpr auto builtin_vertex_color_attribute_name = "vr_vertex_color";
	constexpr auto builtin_vertex_uv_attribute_name = "vr_vertex_uv";

	constexpr auto builtin_mvp_uniform_name = "vr_mvp";
	constexpr auto builtin_view_uniform_name = "vr_view";
	constexpr auto builtin_model_uniform_name = "vr_model";
	constexpr auto builtin_texture_sampler_uniform_name = "vr_texture_sampler";

	void load_uniform(const vr::gl::shader_program& program, const vr::gl::uniform& uniform)
	{
		const auto& uniforms = program.get_uniform_names();
		if (std::find(uniforms.begin(), uniforms.end(), uniform.name) != uniforms.end())
		{
			const auto location = glGetUniformLocation(program.get_id(), uniform.name.c_str());

			using ut = vr::gl::uniform_type;
			switch (uniform.type)
			{
			case ut::mat4fv:
			{
				glUniformMatrix4fv(location, 1, GL_FALSE, &uniform.value.mat4fv[0][0]);
				break;
			}
			case ut::vec3f:
			{
				const glm::vec3& value = uniform.value.vec3f;
				glUniform3f(location, value.x, value.y, value.z);
				break;
			}
			case ut::vec1i:
			{
				glUniform1i(location, uniform.value.vec1i);
				break;
			}
			}
		}
		else
		{
			if (uniform.name != builtin_mvp_uniform_name && uniform.name != builtin_view_uniform_name &&
				uniform.name != builtin_model_uniform_name && uniform.name != builtin_texture_sampler_uniform_name)
			{
				std::cout << "Uniform " << uniform.name << " not found\n";
			}
		}
	}

	void load_builtin_uniforms(const vr::gl::loaded_shader* shader, const vr::object3d* object, const vr::camera& camera)
	{
		const auto model_transformation = object->get_transformation_matrix();

		vr::gl::uniform mvp_uniform;
		mvp_uniform.name = builtin_mvp_uniform_name;
		mvp_uniform.type = vr::gl::uniform_type::mat4fv;
		mvp_uniform.value.mat4fv = camera.get_projection_matrix() * camera.get_view_matrix() * model_transformation;
		load_uniform(shader->program, mvp_uniform);

		vr::gl::uniform view_uniform;
		view_uniform.name = builtin_view_uniform_name;
		view_uniform.type = vr::gl::uniform_type::mat4fv;
		view_uniform.value.mat4fv = camera.get_view_matrix();
		load_uniform(shader->program, view_uniform);

		vr::gl::uniform model_uniform;
		model_uniform.name = builtin_model_uniform_name;
		model_uniform.type = vr::gl::uniform_type::mat4fv;
		model_uniform.value.mat4fv = model_transformation;
		load_uniform(shader->program, model_uniform);

		vr::gl::uniform texture_sampler_uniform;
		texture_sampler_uniform.name = builtin_texture_sampler_uniform_name;
		mvp_uniform.type = vr::gl::uniform_type::vec1i;
		mvp_uniform.value.vec1i = 0;
		load_uniform(shader->program, texture_sampler_uniform);
	}
}

namespace vr::gl
{
	loaded_geometry load_geometry(const geometry* geometry)
	{
		loaded_geometry loaded_geometry;
		vr::gl::loaded_vertex_array_object vao;

		glGenVertexArrays(1, &vao.id);
		glBindVertexArray(vao.id);

		glGenBuffers(1, &vao.buffer.id);
		glBindBuffer(GL_ARRAY_BUFFER, vao.buffer.id);
		glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(vr::geometry::vertex_type), geometry->vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &vao.indices.id);
		glBindBuffer(GL_ARRAY_BUFFER, vao.indices.id);
		glBufferData(GL_ARRAY_BUFFER, geometry->indices.size() * sizeof(vr::geometry::index_type), geometry->indices.data(), GL_STATIC_DRAW);
		vao.indices_size = geometry->indices.size();

		loaded_geometry.vao = vao;

		return loaded_geometry;
	}

	vr::gl::loaded_shader load_shader(const opengl_shader& shader)
	{
		vr::gl::loaded_shader loaded_shader;
		loaded_shader.vertex = vr::gl::shader(vr::gl::shader::type::vertex, shader.get_vertex_shader_source());
		loaded_shader.fragment = vr::gl::shader(vr::gl::shader::type::fragment, shader.get_fragment_shader_source());
		loaded_shader.program = vr::gl::shader_program(loaded_shader.vertex, loaded_shader.fragment);

		return loaded_shader;
	}

	vr::gl::loaded_texture load_texture(const texture* texture)
	{
		return { load_texture(texture->get_path()) };
	}

	renderer::renderer()
		: m_cache(std::make_unique<renderer_cache>())
	{}

	renderer::~renderer()
	{}

	void renderer::render(vr::scene& scene, const vr::camera& camera)
	{
		for (auto object : scene.get_objects())
		{
			load_object(object);
		}

		for (auto object : scene.get_objects())
		{
			render_object(object, camera);
		}
	}

	void renderer::load_object(object3d* object)
	{
		for (const auto mesh : object->get_meshes())
		{
			loaded_geometry* geometry = nullptr;
			loaded_shader* shader = nullptr;
			loaded_texture* texture = nullptr;

			if (!m_cache->get(mesh->get_geometry()))
			{
				geometry = m_cache->set(mesh->get_geometry(), load_geometry(mesh->get_geometry()));
			}
			if (const auto opengl_shader = &static_cast<const opengl_shader_material*>(mesh->get_material())->get_shader(); !m_cache->get(opengl_shader))
			{
				shader = m_cache->set(opengl_shader, load_shader(*opengl_shader));
			}
			if (!m_cache->get(mesh->get_texture()))
			{
				texture = m_cache->set(mesh->get_texture(), load_texture(mesh->get_texture()));
			}
		}

		for (auto child : object->get_children())
		{
			load_object(child);
		}
	}

	void renderer::render_object(const object3d* object, const vr::camera& camera)
	{
		for (const auto* mesh : object->get_meshes())
		{
			const auto geometry = m_cache->get(mesh->get_geometry());
			const auto shader = m_cache->get(&static_cast<const opengl_shader_material*>(mesh->get_material())->get_shader());
			const auto texture = m_cache->get(mesh->get_texture());

			if (texture->id != m_last_shader_id)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->id);
				m_last_texture_id = texture->id;
			}

			if (shader->program.get_id() != m_last_shader_id)
			{
				glUseProgram(shader->program.get_id());
				m_last_shader_id = shader->program.get_id();
			}

			load_builtin_uniforms(shader, object, camera);

			const auto gl_shader = static_cast<const opengl_shader_material*>(mesh->get_material());
			if (gl_shader->get_uniforms())
			{
				for (const auto& uniform : *gl_shader->get_uniforms())
				{
					load_uniform(shader->program, uniform);
				}
			}

			glBindVertexArray(geometry->vao.id);
			glBindBuffer(GL_ARRAY_BUFFER, geometry->vao.buffer.id);

			const auto& attributes = shader->program.get_attribute_names();
			if (std::find(attributes.begin(), attributes.end(), builtin_vertex_position_attribute_name) != attributes.end())
			{
				const auto position_attribute_location = glGetAttribLocation(shader->program.get_id(), builtin_vertex_position_attribute_name);
				glEnableVertexAttribArray(position_attribute_location);
				const auto position_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::position);
				glVertexAttribPointer(position_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(position_offset));
			}

			if (std::find(attributes.begin(), attributes.end(), builtin_vertex_normal_attribute_name) != attributes.end())
			{
				const auto normal_attribute_location = glGetAttribLocation(shader->program.get_id(), builtin_vertex_normal_attribute_name);
				glEnableVertexAttribArray(normal_attribute_location);
				const auto normal_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::normal);
				glVertexAttribPointer(normal_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(normal_offset));
			}

			if (std::find(attributes.begin(), attributes.end(), builtin_vertex_color_attribute_name) != attributes.end())
			{
				const auto color_attribute_location = glGetAttribLocation(shader->program.get_id(), builtin_vertex_color_attribute_name);
				glEnableVertexAttribArray(color_attribute_location);
				const auto color_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::color);
				glVertexAttribPointer(color_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(color_offset));
			}

			if (std::find(attributes.begin(), attributes.end(), builtin_vertex_uv_attribute_name) != attributes.end())
			{
				const auto uv_attribute_location = glGetAttribLocation(shader->program.get_id(), builtin_vertex_uv_attribute_name);
				glEnableVertexAttribArray(uv_attribute_location);
				const auto uv_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::texcoords);
				glVertexAttribPointer(uv_attribute_location, 2, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(uv_offset));
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->vao.indices.id);

			glDrawElements(GL_TRIANGLES, geometry->vao.indices_size, GL_UNSIGNED_SHORT, nullptr);
		}

		for (const auto child : object->get_children())
		{
			render_object(child, camera);
		}
	}
}
