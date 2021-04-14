#include "renderer.hpp"
#include "opengl_shader.hpp"
#include "gpu_objects.hpp"
#include "renderer_cache.hpp"
#include "texture_loader.hpp"
#include <shader_material.hpp>

#include <map>

namespace
{
	void load_uniform(const vr::gl::shader_program& program, const vr::gl::uniform& uniform)
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

	vr::gl::loaded_shader load_shader(const shader_material* material)
	{
		const auto shader = static_cast<const opengl_shader*>(material);
		vr::gl::loaded_shader loaded_shader;
		loaded_shader.vertex = vr::gl::shader(vr::gl::shader::type::vertex, shader->get_vertex_shader_source());
		loaded_shader.fragment = vr::gl::shader(vr::gl::shader::type::fragment, shader->get_fragment_shader_source());
		loaded_shader.program = vr::gl::shader_program(loaded_shader.vertex, loaded_shader.fragment);

		return loaded_shader;
	}

	vr::gl::loaded_texture load_texture(const texture* texture)
	{
		return { load_dds(texture->get_path()) };
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
			if (!m_cache->get(mesh->get_material()))
			{
				shader = m_cache->set(mesh->get_material(), load_shader(mesh->get_material()));
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
			const auto shader = m_cache->get(mesh->get_material());
			const auto texture = m_cache->get(mesh->get_texture());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->id);

			glUseProgram(shader->program.get_id());
			const auto gl_shader = static_cast<const opengl_shader*>(mesh->get_material());

			uniform mvp_uniform;
			mvp_uniform.name = "mvp";
			mvp_uniform.type = uniform_type::mat4fv;
			mvp_uniform.value.mat4fv = camera.get_projection_matrix() * camera.get_view_matrix() * glm::mat4(1.0f);
			load_uniform(shader->program, mvp_uniform);

			uniform texture_sampler_uniform;
			texture_sampler_uniform.name = "texture_sampler";
			mvp_uniform.type = uniform_type::vec1i;
			mvp_uniform.value.vec1i = 0;

			for (const auto& uniform : gl_shader->get_uniforms())
			{
				load_uniform(shader->program, uniform);
			}

			glBindVertexArray(geometry->vao.id);
			glBindBuffer(GL_ARRAY_BUFFER, geometry->vao.buffer.id);

			if (true)
			{
				const auto position_attribute_location = glGetAttribLocation(shader->program.get_id(), "vertex_position_model");
				glEnableVertexAttribArray(position_attribute_location);
				const auto position_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::position);
				glVertexAttribPointer(position_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(position_offset));
			}

			if (true)
			{
				const auto normal_attribute_location = glGetAttribLocation(shader->program.get_id(), "vertex_normal_model");
				glEnableVertexAttribArray(normal_attribute_location);
				const auto normal_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::normal);
				glVertexAttribPointer(normal_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(normal_offset));
			}

			if (false)
			{
				const auto color_attribute_location = glGetAttribLocation(shader->program.get_id(), "vertex_color");
				glEnableVertexAttribArray(color_attribute_location);
				const auto color_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::color);
				glVertexAttribPointer(color_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(color_offset));
			}

			if (true)
			{
				const auto uv_attribute_location = glGetAttribLocation(shader->program.get_id(), "vertex_uv_model");
				glEnableVertexAttribArray(uv_attribute_location);
				const auto uv_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::texcoords);
				glVertexAttribPointer(uv_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(uv_offset));
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

//const auto& on_gpu = m_loaded_gpu_objects[&mesh];
//const auto& projection_matrix = camera.get_projection_matrix();
//const auto& view_matrix = camera.get_view_matrix();
//const auto& model_matrix = glm::mat4(1.0f);// mesh.get_model_matrix();

//glUseProgram(on_gpu.shader->program.get_id());
//const auto mvp = projection_matrix * view_matrix * model_matrix;
//const auto mvp_matrix_uniform_location = glGetUniformLocation(on_gpu.shader->program.get_id(), "mvp_matrix");
//glUniformMatrix4fv(mvp_matrix_uniform_location, 1, GL_FALSE, &mvp[0][0]);


//loaded_geometry* geometry = nullptr;
//glBindVertexArray(geometry->vao.id);
//glBindBuffer(GL_ARRAY_BUFFER, on_gpu.vao->buffer.id);

//if (true)
//{
//	const auto position_attribute_location = glGetAttribLocation(on_gpu.shader->program.get_id(), "vertex_position_model");
//	glEnableVertexAttribArray(position_attribute_location);
//	const auto position_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::position);
//	glVertexAttribPointer(position_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(position_offset));
//}

//if (true)
//{
//	const auto normal_attribute_location = glGetAttribLocation(on_gpu.shader->program.get_id(), "vertex_normal_model");
//	glEnableVertexAttribArray(normal_attribute_location);
//	const auto normal_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::normal);
//	glVertexAttribPointer(normal_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(normal_offset));
//}

//if (true)
//{
//	const auto color_attribute_location = glGetAttribLocation(on_gpu.shader->program.get_id(), "vertex_color");
//	glEnableVertexAttribArray(color_attribute_location);
//	const auto color_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::color);
//	glVertexAttribPointer(color_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(color_offset));
//}

//if (true)
//{
//	const auto uv_attribute_location = glGetAttribLocation(on_gpu.shader->program.get_id(), "vertex_uv_model");
//	glEnableVertexAttribArray(uv_attribute_location);
//	const auto uv_offset = offsetof(vr::mesh::geometry_type::vertex_type, vr::mesh::geometry_type::vertex_type::texcoords);
//	glVertexAttribPointer(uv_attribute_location, 3, GL_FLOAT, GL_FALSE, sizeof(vr::mesh::geometry_type::vertex_type), reinterpret_cast<const void*>(uv_offset));
//}

//glBindBuffer(GL_ARRAY_BUFFER, on_gpu.vao->indices.id);

//glDrawElements(GL_TRIANGLES, on_gpu.vao->indices_size, GL_UNSIGNED_SHORT, nullptr);
//}

	//object_on_gpu renderer::buffer_object(const mesh& mesh)
	//{
	//	object_on_gpu on_gpu;
	//	on_gpu.m_corresponding_object = &mesh;

	//	if (auto loaded_shader_it = m_loaded_shaders.find(&mesh.get_material()); loaded_shader_it == m_loaded_shaders.end())
	//	{
	//		auto loaded_shader = load_material(mesh.get_material());
	//		auto [it, result] = m_loaded_shaders.emplace(&mesh.get_material(), std::move(loaded_shader));
	//		on_gpu.shader = &it->second;
	//	}
	//	else
	//	{
	//		on_gpu.shader = &loaded_shader_it->second;
	//	}

	//	if (auto loaded_geometry_it = m_loaded_geometry.find(&mesh.get_geometry()); loaded_geometry_it == m_loaded_geometry.end())
	//	{
	//		auto loaded_buffers = load_geometry(mesh.get_geometry());
	//		auto [it, result] = m_loaded_geometry.emplace(&mesh.get_geometry(), loaded_buffers);
	//		on_gpu.vao = &it->second;
	//	}
	//	else
	//	{
	//		on_gpu.vao = &loaded_geometry_it->second;
	//	}

	//	return on_gpu;
	//}