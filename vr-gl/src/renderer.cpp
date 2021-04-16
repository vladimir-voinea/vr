#include "renderer.hpp"
#include "opengl_shader_material.hpp"
#include "cube_texture_material.hpp"
#include "gpu_objects.hpp"
#include "renderer_cache.hpp"
#include "texture_loader.hpp"
#include "opengl_debug_callback.hpp"
#include <shader_material.hpp>

#include <spdlog/spdlog.h>

#include <map>
#include <algorithm>
#include <numeric>

namespace
{
	constexpr auto builtin_vertex_position_attribute_name = "vr_vertex_position";
	constexpr auto builtin_vertex_normal_attribute_name = "vr_vertex_normal";
	constexpr auto builtin_vertex_color_attribute_name = "vr_vertex_color";
	constexpr auto builtin_vertex_uv_attribute_name = "vr_vertex_uv";

	constexpr auto builtin_mvp_uniform_name = "vr_mvp";
	constexpr auto builtin_projection_uniform_name = "vr_projection";
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
			case ut::vec4f:
			{
				const glm::vec4& value = uniform.value.vec4f;
				glUniform4f(location, value.x, value.y, value.z, value.w);
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
			if (uniform.name != builtin_mvp_uniform_name && uniform.name != builtin_projection_uniform_name &&
				uniform.name != builtin_view_uniform_name && uniform.name != builtin_model_uniform_name && 
				uniform.name != builtin_texture_sampler_uniform_name)
			{
				spdlog::error("Uniform {0} not found", uniform.name);
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

		vr::gl::uniform projection_uniform;
		projection_uniform.name = builtin_projection_uniform_name;
		projection_uniform.type = vr::gl::uniform_type::mat4fv;
		projection_uniform.value.mat4fv = camera.get_projection_matrix();
		load_uniform(shader->program, projection_uniform);

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
		return { vr::gl::load_texture(texture) };
	}

	vr::gl::loaded_texture load_texture(const vr::cube_texture* texture)
	{
		return { vr::gl::load_texture(texture) };
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

	void initialize_glew()
	{
		glewExperimental = GL_TRUE;
		const auto glew_initialization = glewInit();
		if (glew_initialization != GLEW_OK)
		{
			throw std::runtime_error("Could not initialize glew");
		}
	}
}

namespace vr::gl
{


	renderer::renderer(const renderer_settings& settings)
		: m_settings(settings)
		, m_cache(std::make_unique<renderer_cache>())
	{
		initialize_glew();

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(opengl_debug_callback, nullptr);

		if (m_settings.cull_faces)
		{
			glEnable(GL_CULL_FACE);
		}

		if (m_settings.wireframe_mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	renderer::~renderer()
	{}

	void renderer::render(vr::scene& scene, const vr::camera& camera)
	{
		if (m_settings.clear_color)
		{
			const auto clear_color = glm::normalize(glm::vec3(m_settings.clear_color->x, m_settings.clear_color->y, m_settings.clear_color->z));
			glClearColor(clear_color.r, clear_color.g, clear_color.b, 0.0f);
		}

		for (auto object : scene.get_objects())
		{
			load_object(object);
		}

		for (auto object : scene.get_objects())
		{
			render_object(object, camera);
		}

		if (m_settings.skybox)
		{
			load_skybox(m_settings.skybox.get());
			render_skybox(m_settings.skybox.get(), camera);
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

	void renderer::render_skybox(const skybox* skybox, const camera& camera)
	{
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);

		auto cube_texture = m_cache->get(skybox->get_texture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture->id);

		const auto shader = m_cache->get(&static_cast<const opengl_shader_material*>(skybox->get_material())->get_shader());
		glUseProgram(shader->program.get_id());
		m_last_shader_id = shader->program.get_id();

		vr::gl::uniform projection_uniform;
		projection_uniform.name = builtin_projection_uniform_name;
		projection_uniform.type = vr::gl::uniform_type::mat4fv;
		projection_uniform.value.mat4fv = camera.get_projection_matrix();
		load_uniform(shader->program, projection_uniform);

		vr::gl::uniform view_uniform;
		view_uniform.name = builtin_view_uniform_name;
		view_uniform.type = vr::gl::uniform_type::mat4fv;
		view_uniform.value.mat4fv = glm::mat4(glm::mat3(camera.get_view_matrix()));
		load_uniform(shader->program, view_uniform);
		
		const auto gl_shader = static_cast<const opengl_shader_material*>(skybox->get_material());
		if (gl_shader->get_uniforms())
		{
			for (const auto& uniform : *gl_shader->get_uniforms())
			{
				load_uniform(shader->program, uniform);
			}
		}

		const auto geometry = m_cache->get(skybox->get_geometry());

		glBindVertexArray(geometry->vao.id);
		glBindBuffer(GL_ARRAY_BUFFER, geometry->vao.buffer.id);

		ptrdiff_t accumulated_offset = 0;
		std::vector<GLint> bound_attributes;
		const auto& shader_attributes = shader->program.get_attribute_names();
		for (const auto& attribute : skybox->get_geometry()->attributes)
		{
			const auto& name = attribute.first;
			if (std::find(shader_attributes.begin(), shader_attributes.end(), name) != shader_attributes.end())
			{
				const auto attribute_location = glGetAttribLocation(shader->program.get_id(), name.c_str());
				glEnableVertexAttribArray(attribute_location);
				glVertexAttribPointer(attribute_location, geometry->vao.buffer.loaded_attributes[name].components,
					::convert_to_gl_enum(geometry->vao.buffer.loaded_attributes[name].type), GL_FALSE, 0,
					static_cast<const void*>(static_cast<const uint8_t*>(nullptr) + geometry->vao.buffer.loaded_attributes[name].start));
				accumulated_offset += attribute.second.data.size();
				bound_attributes.push_back(attribute_location);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->vao.indices.id);

		glDrawElements(GL_TRIANGLES, geometry->vao.indices_size, GL_UNSIGNED_SHORT, nullptr);

		for (const auto& bound_attribute : bound_attributes)
		{
			glDisableVertexAttribArray(bound_attribute);
		}

		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
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
			if (mesh->get_texture() && !m_cache->get(mesh->get_texture()))
			{
				texture = m_cache->set(mesh->get_texture(), ::load_texture(mesh->get_texture()));
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

			if (mesh->get_texture())
			{
				const auto texture = m_cache->get(mesh->get_texture());
				if (texture->id != m_last_shader_id)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texture->id);
					m_last_texture_id = texture->id;
				}
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

			ptrdiff_t accumulated_offset = 0;
			std::vector<GLint> bound_attributes;
			const auto& shader_attributes = shader->program.get_attribute_names();
			for (const auto& attribute : mesh->get_geometry()->attributes)
			{
				const auto& name = attribute.first;
				if (std::find(shader_attributes.begin(), shader_attributes.end(), name) != shader_attributes.end())
				{
					const auto attribute_location = glGetAttribLocation(shader->program.get_id(), name.c_str());
					glEnableVertexAttribArray(attribute_location);
					glVertexAttribPointer(attribute_location, geometry->vao.buffer.loaded_attributes[name].components,
						::convert_to_gl_enum(geometry->vao.buffer.loaded_attributes[name].type), GL_FALSE, 0,
						static_cast<const void*>(static_cast<const uint8_t*>(nullptr) + geometry->vao.buffer.loaded_attributes[name].start));
					accumulated_offset += attribute.second.data.size();
					bound_attributes.push_back(attribute_location);
				}
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->vao.indices.id);

			glDrawElements(GL_TRIANGLES, geometry->vao.indices_size, GL_UNSIGNED_SHORT, nullptr);

			for (const auto& bound_attribute : bound_attributes)
			{
				glDisableVertexAttribArray(bound_attribute);
			}
		}

		for (const auto child : object->get_children())
		{
			render_object(child, camera);
		}
	}
}
