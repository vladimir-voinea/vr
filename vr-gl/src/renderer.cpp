#include "renderer.hpp"
#include "opengl_shader_material.hpp"
#include "cube_texture_material.hpp"
#include "gpu_objects.hpp"
#include "renderer_cache.hpp"
#include "texture_loader.hpp"
#include "opengl_debug_callback.hpp"
#include "vr-opengl.h"
#include "builtin_uniforms.hpp"
#include "uniform_loader.hpp"
#include <shader_material.hpp>

#include <spdlog/spdlog.h>

#include <glm/gtx/matrix_decompose.hpp> 

#include <map>
#include <algorithm>
#include <numeric>

namespace
{
	void load_builtin_uniforms(const vr::gl::loaded_shader* shader, const vr::object3d* object, const vr::camera& camera)
	{
		using namespace vr::gl;

		const auto model_matrix = object->get_transformation_matrix();
		const auto view_matrix = camera.get_view_matrix();
		const auto projection_matrix = camera.get_projection_matrix();

		uniform_loader loader(shader->program);

		if (loader.has_uniform(builtin_mvp_uniform_name))
		{
			const auto mvp_matrix = projection_matrix * view_matrix * model_matrix;
			vr::uniform mvp_uniform;
			mvp_uniform.name = builtin_mvp_uniform_name;
			mvp_uniform.type = vr::uniform_type::mat4fv;
			mvp_uniform.value.mat4fv = mvp_matrix;
			loader.load_uniform(mvp_uniform);
		}

		if (loader.has_uniform(builtin_projection_uniform_name))
		{
			vr::uniform projection_uniform;
			projection_uniform.name = builtin_projection_uniform_name;
			projection_uniform.type = vr::uniform_type::mat4fv;
			projection_uniform.value.mat4fv = projection_matrix;
			loader.load_uniform(projection_uniform);
		}

		if (loader.has_uniform(builtin_view_uniform_name))
		{
			vr::uniform view_uniform;
			view_uniform.name = builtin_view_uniform_name;
			view_uniform.type = vr::uniform_type::mat4fv;
			view_uniform.value.mat4fv = view_matrix;
			loader.load_uniform(view_uniform);
		}

		if (loader.has_uniform(builtin_model_uniform_name))
		{
			vr::uniform model_uniform;
			model_uniform.name = builtin_model_uniform_name;
			model_uniform.type = vr::uniform_type::mat4fv;
			model_uniform.value.mat4fv = model_matrix;
			loader.load_uniform(model_uniform);
		}

		if (loader.has_uniform(builtin_view_position_uniform_name))
		{
			const auto view_matrix_inverse = glm::inverse(view_matrix);

			vr::uniform view_position_uniform;
			view_position_uniform.name = builtin_view_position_uniform_name;
			view_position_uniform.type = vr::uniform_type::vec3f;
			glm::vec3 scale;
			glm::quat orientation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(view_matrix_inverse, scale, orientation, translation, skew, perspective);
			view_position_uniform.value.vec3f = translation;
			loader.load_uniform(view_position_uniform);
		}

		if (loader.has_uniform(builtin_modelview_uniform_name))
		{
			const auto modelview_matrix = view_matrix * model_matrix;
			vr::uniform modelview_uniform;
			modelview_uniform.name = builtin_modelview_uniform_name;
			modelview_uniform.type = vr::uniform_type::mat4fv;
			modelview_uniform.value.mat4fv = modelview_matrix;
			loader.load_uniform(modelview_uniform);
		}

		if (loader.has_uniform(builtin_normal_uniform_name))
		{
			const auto normal_matrix = glm::transpose(glm::inverse(model_matrix));
			vr::uniform normal_uniform;
			normal_uniform.name = builtin_normal_uniform_name;
			normal_uniform.type = vr::uniform_type::mat3fv;
			normal_uniform.value.mat3fv = normal_matrix;
			loader.load_uniform(normal_uniform);
		}
	}

	vr::gl::loaded_geometry load_geometry(const vr::geometry* geometry)
	{
		vr::gl::loaded_geometry loaded_geometry;
		vr::gl::loaded_vertex_array_object vao;

		glGenVertexArrays(1, &vao.id);
		glBindVertexArray(vao.id);

		GLsizeiptr buffer_size = 0;
		for (const auto& attribute : geometry->attributes)
		{
			buffer_size += attribute.second.data.size();
		}

		glGenBuffers(1, &vao.buffer.id);
		glBindBuffer(GL_ARRAY_BUFFER, vao.buffer.id);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);

		uint64_t accumulated_offset = 0;
		for (const auto& attribute : geometry->attributes)
		{
			glBufferSubData(GL_ARRAY_BUFFER, accumulated_offset, attribute.second.data.size(), attribute.second.data.data());

			vr::gl::vertex_buffer::attribute_layout layout;
			layout.start = accumulated_offset;
			layout.size = attribute.second.data.size();
			layout.type = attribute.second.type;
			layout.components = attribute.second.components;
			vao.buffer.loaded_attributes[attribute.first] = layout;

			accumulated_offset += attribute.second.data.size();
		}

		glGenBuffers(1, &vao.indices.id);
		glBindBuffer(GL_ARRAY_BUFFER, vao.indices.id);
		glBufferData(GL_ARRAY_BUFFER, geometry->indices.size() * sizeof(decltype(geometry->indices)::value_type), geometry->indices.data(), GL_STATIC_DRAW);
		vao.indices_size = geometry->indices.size();

		loaded_geometry.vao = vao;

		const auto size_kb = accumulated_offset / 1024;
		const auto size_mb = static_cast<float>(size_kb) / 1024.f;
		const auto n_triangles = geometry->indices.size() / 3;
		const auto n_triangles_k = static_cast<float>(n_triangles) / 1000.f;
		spdlog::info("Loaded mesh of {} KB ({} MB) with {} triangles ({}k)", size_kb,
			size_mb, n_triangles, n_triangles_k);

		return loaded_geometry;
	}

	vr::gl::loaded_shader load_shader(const vr::gl::opengl_shader& shader)
	{
		vr::gl::loaded_shader loaded_shader;
		loaded_shader.vertex = vr::gl::shader(vr::gl::shader::type::vertex, shader.get_vertex_shader_source());
		loaded_shader.fragment = vr::gl::shader(vr::gl::shader::type::fragment, shader.get_fragment_shader_source());
		loaded_shader.program = vr::gl::shader_program(loaded_shader.vertex, loaded_shader.fragment);

		return loaded_shader;
	}

	vr::gl::loaded_texture load_texture(const vr::texture* texture)
	{
		return { vr::gl::load_texture(texture), GL_TEXTURE_2D };
	}

	vr::gl::loaded_texture load_texture(const vr::cube_texture* texture)
	{
		return { vr::gl::load_texture(texture), GL_TEXTURE_CUBE_MAP };
	}

	int convert_to_gl_enum(const vr::attribute::data_type& type)
	{
		int result = 0;
		switch (type)
		{
		case vr::attribute::data_type::t_float:
		{
			result = GL_FLOAT;
			break;
		}
		default:
		{
			assert(false);
		}
		}

		return result;
	}
}

namespace vr::gl
{
	renderer::renderer(const renderer_settings& settings)
		: m_settings(settings)
		, m_cache(std::make_unique<renderer_cache>())
	{
		const char* glsl_version = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
		spdlog::info("GLSL: {0}", glsl_version);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(opengl_debug_callback, nullptr);

		if (m_settings.cull_faces)
		{
			glEnable(GL_CULL_FACE);
		}

		if (m_settings.wireframe_mode)
		{
#ifndef __ANDROID__
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_SCISSOR_TEST);
	}

	renderer::~renderer()
	{}

	void renderer::render(vr::scene& scene, const vr::camera& camera)
	{
		glViewport(m_settings.viewport.get_x0(), m_settings.viewport.get_y0(), m_settings.viewport.get_x1(), m_settings.viewport.get_y1());
		glScissor(m_settings.viewport.get_x0(), m_settings.viewport.get_y0(), m_settings.viewport.get_x1(), m_settings.viewport.get_y1());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_settings.clear_color)
		{
			const auto clear_color = glm::normalize(glm::vec3(m_settings.clear_color->x, m_settings.clear_color->y, m_settings.clear_color->z));
			glClearColor(clear_color.r, clear_color.g, clear_color.b, 0.0f);
		}

		if (m_settings.skybox)
		{
			load_skybox(m_settings.skybox.get());
		}

		scene.traverse([this, &camera](vr::object3d* node)
			{
				load_object(node);
				render_object(node, camera);
			});

		if (m_settings.skybox)
		{
			render_skybox(m_settings.skybox.get(), camera);
		}
	}

	void renderer::load_object(object3d* object)
	{
		if (object->has_geometry())
		{
			for (const auto mesh : object->get_meshes())
			{
				if (!m_cache->get(mesh->get_geometry()))
				{
					m_cache->set(mesh->get_geometry(), load_geometry(mesh->get_geometry()));
				}

				auto material = static_cast<const opengl_shader_material*>(mesh->get_material());
				if (const auto& opengl_shader = material->get_shader(); !m_cache->get(&opengl_shader))
				{
					m_cache->set(&opengl_shader, load_shader(opengl_shader));	
				}

				const auto& textures = material->get_textures();
				for (const auto texture : textures)
				{
					if (!texture)
					{
						spdlog::error("Attempting to load null texture");
						assert(false);
					}
					if (!m_cache->get(texture))
					{
						m_cache->set(texture, ::load_texture(texture));
					}
				}
			}
		}

		for (const auto& child : object->get_children())
		{
			load_object(child.get());
		}
	}

	void renderer::load_skybox(const skybox* skybox)
	{
		loaded_texture* texture = nullptr;
		loaded_geometry* geometry = nullptr;
		loaded_shader* shader = nullptr;
		if (!m_cache->get(skybox->get_texture()))
		{
			texture = m_cache->set(skybox->get_texture(), ::load_texture(skybox->get_texture()));
		}
		if (!m_cache->get(m_settings.skybox->get_geometry()))
		{
			geometry = m_cache->set(skybox->get_geometry(), load_geometry(skybox->get_geometry()));
		}
		if (const auto opengl_shader = &static_cast<const opengl_shader_material*>(skybox->get_material())->get_shader(); !m_cache->get(opengl_shader))
		{
			shader = m_cache->set(opengl_shader, load_shader(*opengl_shader));
		}
	}

	void renderer::activate_texture(const loaded_texture* texture, unsigned int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture->id);
	}

	void renderer::deactivate_texture_unit(unsigned int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void renderer::activate_shader(const loaded_shader* shader)
	{
		if (shader->program.get_id() != m_last_shader_id)
		{
			glUseProgram(shader->program.get_id());
			m_last_shader_id = shader->program.get_id();
		}
	}

	void renderer::load_shader_uniforms(const opengl_shader_material* material, const loaded_shader* shader)
	{
		uniform_loader loader(shader->program);

		for (const auto& uniform : material->get_uniforms())
		{
			loader.load_uniform(uniform);
		}
	}

	void renderer::render_geometry(const vr::geometry* geometry, const loaded_shader* shader)
	{
		const auto geometry_on_gpu = m_cache->get(geometry);
		glBindVertexArray(geometry_on_gpu->vao.id);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_on_gpu->vao.buffer.id);

		std::vector<GLint> bound_attributes;
		const auto& shader_attributes = shader->program.get_attribute_names();
		for (const auto& attribute : geometry->attributes)
		{
			const auto& name = attribute.first;
			if (std::find(shader_attributes.begin(), shader_attributes.end(), name) != shader_attributes.end())
			{
				const auto attribute_location = glGetAttribLocation(shader->program.get_id(), name.c_str());
				glEnableVertexAttribArray(attribute_location);
				glVertexAttribPointer(attribute_location, geometry_on_gpu->vao.buffer.loaded_attributes[name].components,
					::convert_to_gl_enum(geometry_on_gpu->vao.buffer.loaded_attributes[name].type), GL_FALSE, 0,
					static_cast<const void*>(static_cast<const uint8_t*>(nullptr) + geometry_on_gpu->vao.buffer.loaded_attributes[name].start));
				bound_attributes.push_back(attribute_location);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_on_gpu->vao.indices.id);

		glDrawElements(GL_TRIANGLES, geometry_on_gpu->vao.indices_size, GL_UNSIGNED_INT, nullptr);

		for (const auto& bound_attribute : bound_attributes)
		{
			glDisableVertexAttribArray(bound_attribute);
		}
	}

	void renderer::render_object(object3d* object, const vr::camera& camera)
	{
		object->on_before_render();

		if (object->has_geometry())
		{
			for (auto* mesh : object->get_meshes())
			{
				const auto material = static_cast<const opengl_shader_material*>(mesh->get_material());
				const auto shader = m_cache->get(&material->get_shader());

				const auto n_textures = material->get_textures().size();
				for (auto i = 0u; i < n_textures; ++i)
				{
					if (const auto texture = m_cache->get(material->get_textures()[i]); texture)
					{
						activate_texture(texture, i);
					}

				}
				
				activate_shader(shader);
				load_builtin_uniforms(shader, object, camera);
				load_shader_uniforms(static_cast<const opengl_shader_material*>(mesh->get_material()), shader);

				render_geometry(mesh->get_geometry(), shader);

				for (auto i = 0u; i < n_textures; ++i)
				{
					deactivate_texture_unit(i);
				}
			}
		}

		for (auto& child : object->get_children())
		{
			render_object(child.get(), camera);
		}
	}

	void renderer::render_skybox(const skybox* skybox, const camera& camera)
	{
		glDepthFunc(GL_LEQUAL);

		if (m_settings.cull_faces)
		{
			glDisable(GL_CULL_FACE);
		}

		auto cube_texture = m_cache->get(skybox->get_texture());
		activate_texture(cube_texture, 0);

		const auto shader = m_cache->get(&static_cast<const opengl_shader_material*>(skybox->get_material())->get_shader());
		activate_shader(shader);
		uniform_loader loader(shader->program);

		vr::uniform projection_uniform;
		projection_uniform.name = builtin_projection_uniform_name;
		projection_uniform.type = vr::uniform_type::mat4fv;
		projection_uniform.value.mat4fv = camera.get_projection_matrix();
		loader.load_uniform(projection_uniform);

		vr::uniform view_uniform;
		view_uniform.name = builtin_view_uniform_name;
		view_uniform.type = vr::uniform_type::mat4fv;
		view_uniform.value.mat4fv = glm::mat4(glm::mat3(camera.get_view_matrix()));
		loader.load_uniform(view_uniform);

		load_shader_uniforms(static_cast<const opengl_shader_material*>(skybox->get_material()), shader);

		render_geometry(skybox->get_geometry(), shader);

		if (m_settings.cull_faces)
		{
			glEnable(GL_CULL_FACE);
		}

		glDepthFunc(GL_LESS);
	}
}
