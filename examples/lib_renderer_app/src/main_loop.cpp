#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include <orthographic_camera.hpp>
#include "shader_loader.hpp"
#include "shaders.hpp"

#include <cube_texture_material.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>
#include <stdexcept>

#include <android_logging.hpp>

#include <filesystem>

static const auto start_position = glm::vec3{ -2.7872, 1.74459, 9.47206 };
static const auto start_direction = glm::vec3{ 0.115009114, 0.016061125, -0.9932346 };

constexpr auto light_bulb_vshader = R"(
		#version 300 es

		in vec3 vr_vertex_position;
		uniform mat4 vr_mvp;
		out highp vec3 cube_texcoords;

		void main() {
			vec4 position = vr_mvp * vec4(vr_vertex_position, 1.f);
			gl_Position = position;
		}
)";

constexpr auto light_bulb_fshader = R"(
		#version 300 es
		
		out highp vec4 out_color4;
		uniform highp vec3 color;

		void main()
		{
			out_color4 = vec4(color, 0.f);
		}
)";

vr::perspective_camera::settings make_camera_settings(int width, int height)
{
	vr::perspective_camera::settings settings;

	settings.fov = 45.f;
	settings.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	settings.near = 0.1f;
	settings.far = 100.f;

	settings.position = start_position;
	settings.direction = start_direction;

	return settings;
}

void set_orthographic_view_mapping(vr::orthographic_camera::settings& settings, int width, int height)
{
	const auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	if (width > height)
	{
		settings.left = -0.5f * aspect_ratio;
		settings.right = -settings.left;
		settings.bottom = -0.5f;
		settings.top = 0.5f;
	}
	else
	{
		settings.left = -0.5f;
		settings.right = 0.5f;
		settings.bottom = -0.5f * aspect_ratio;
		settings.top = -settings.bottom;
	}
}

vr::orthographic_camera::settings make_orthographic_camera_settings(int width, int height)
{
	vr::orthographic_camera::settings settings;

	set_orthographic_view_mapping(settings, width, height);

	settings.near = 0.1f;
	settings.far = 100.f;

	settings.fov = 45.f;
	settings.position = start_position;
	settings.direction = start_direction;

	return settings;
}

main_loop::main_loop(const viewport& viewport)
	: m_viewport(viewport)
	, m_perspective_camera_settings(make_camera_settings(m_viewport.width, m_viewport.height))
	, m_orthographic_camera_settings(make_orthographic_camera_settings(m_viewport.width, m_viewport.height))
{
	try
	{
		vr::platform::setup_android_logging();
		init();
	}
	catch (const std::exception& e)
	{
		spdlog::error("Could not initialize main loop: {0}", e.what());
		throw;
	}
}

main_loop::~main_loop()
{
}

void main_loop::initialize_position()
{
}

void main_loop::print_state()
{

}

void main_loop::make_cameras()
{
	m_perspective_camera = std::make_unique<vr::perspective_camera>(m_perspective_camera_settings);
	m_orthographic_camera = std::make_unique<vr::orthographic_camera>(m_orthographic_camera_settings);
}

void main_loop::init()
{
	make_cameras();

	m_cube_texture = std::make_unique<vr::cube_texture>(std::unordered_map<std::string, std::string>{
		{ vr::cube_texture::p_x, "skybox/urban-skyboxes/SaintLazarusChurch2/posx.jpg" },
		{ vr::cube_texture::n_x, "skybox/urban-skyboxes/SaintLazarusChurch2/negx.jpg" },
		{ vr::cube_texture::p_y, "skybox/urban-skyboxes/SaintLazarusChurch2/posy.jpg" },
		{ vr::cube_texture::n_y, "skybox/urban-skyboxes/SaintLazarusChurch2/negy.jpg" },
		{ vr::cube_texture::n_z, "skybox/urban-skyboxes/SaintLazarusChurch2/negz.jpg" },
		{ vr::cube_texture::p_z, "skybox/urban-skyboxes/SaintLazarusChurch2/posz.jpg" }
	});
	m_skybox_material = std::make_unique<vr::gl::cube_texture_material>();

	//m_renderer_settings.skybox = std::make_unique<vr::skybox>(m_skybox_material.get(), m_cube_texture.get());
	m_renderer_settings.clear_color = std::make_unique<glm::vec3>(1, 1, 1);

	m_renderer_settings.cull_faces = true;
	m_renderer_settings.wireframe_mode = false;

	m_renderer_settings.viewport.x0 = m_viewport.x0;
	m_renderer_settings.viewport.y0 = m_viewport.y0;

	m_renderer_settings.viewport.width = m_viewport.width;
	m_renderer_settings.viewport.height = m_viewport.height;

	m_renderer = std::make_unique<vr::gl::renderer>(m_renderer_settings);

	initialize_position();
	add_light_bulb();
}

void main_loop::add_light_bulb()
{
	const glm::vec3 scale_factor = { 0.1, 0.1, 0.1 };

	m_directional_light_model = vr::model::load_model("data/models/uv_sphere.obj");
	m_directional_light = m_directional_light_model.first.get();
	//m_directional_light->scale(scale_factor);
	m_scene.add(m_directional_light);

	m_point_light_model = vr::model::load_model("data/models/torus.obj");
	m_point_light = m_point_light_model.first.get();
	//m_point_light->scale(scale_factor);
	m_scene.add(m_point_light);

	m_spot_light_model = vr::model::load_model("data/models/cone.obj");
	m_spot_light = m_spot_light_model.first.get();
	//m_spot_light->scale(scale_factor);
	m_scene.add(m_spot_light);
}

void main_loop::resize(int width, int height)
{
	m_viewport.width = width;
	m_viewport.height = height;

	spdlog::info("New framebuffer size: {0}, {1}", width, height);
	m_perspective_camera_settings.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	set_orthographic_view_mapping(m_orthographic_camera_settings, width, height);

	m_renderer_settings.viewport.width = width;
	m_renderer_settings.viewport.height = height;
}

vr::camera& main_loop::get_camera()
{
	constexpr auto perspective = true;

	return perspective ? *m_perspective_camera : *m_orthographic_camera;
}

void main_loop::frame(float delta_time, const parameters& parameters)
{
	if (parameters.path != m_scene_model_path)
	{
		if (std::filesystem::exists(parameters.path))
		{
			if (m_scene_model)
			{
				m_scene.remove(m_scene_model);
			}

			if (auto it = m_loaded_models.find(parameters.path); it == m_loaded_models.end())
			{
				auto insert = m_loaded_models.insert(std::make_pair(parameters.path, vr::model::load_model(parameters.path)));
				m_scene_model = insert.first->second.first.get();
			}
			else
			{
				m_scene_model = it->second.first.get();
			}

			m_scene.add(m_scene_model);
			m_scene_model->set_name("Model root");
		}
	}
	transform_model(parameters);

	m_renderer->render(m_scene, get_camera());
}

void create_or_update(std::vector<vr::gl::uniform>& uniforms, const std::string& name, const float& value)
{
	auto uniform_it = std::find_if(uniforms.begin(), uniforms.end(), [&name](const vr::gl::uniform& uniform)
		{
			return uniform.name == name;
		});

	if (uniform_it == uniforms.end())
	{
		vr::gl::uniform uniform;
		uniform.name = name;
		uniform.type = vr::gl::uniform_type::vec1f;
		uniform.value.vec1f = value;
		uniforms.push_back(uniform);
	}
	else
	{
		uniform_it->value.vec1f = value;
	}
}

void create_or_update(std::vector<vr::gl::uniform>& uniforms, const std::string& name, const glm::vec3& value)
{
	auto uniform_it = std::find_if(uniforms.begin(), uniforms.end(), [&name](const vr::gl::uniform& uniform)
		{
			return uniform.name == name;
		});

	if (uniform_it == uniforms.end())
	{
		vr::gl::uniform uniform;
		uniform.name = name;
		uniform.type = vr::gl::uniform_type::vec3f;
		uniform.value.vec3f = value;
		uniforms.push_back(uniform);
	}
	else
	{
		uniform_it->value.vec3f = value;
	}
}

void set_light_parameters(vr::object3d* object, const parameters& parameters, glm::vec3 target)
{
	for (auto& mesh : object->get_meshes())
	{

		vr::gl::opengl_shader_material* material = static_cast<vr::gl::opengl_shader_material*>(mesh->get_material());
		auto& uniforms = material->get_uniforms();


		create_or_update(uniforms, "vr_have_ambient_light", parameters.have_ambient_light);
		create_or_update(uniforms, "vr_ambient_light.color", parameters.ambient_light.color);
		create_or_update(uniforms, "vr_ambient_light.intensity", parameters.ambient_light.intensity);

		create_or_update(uniforms, "vr_have_directional_light", parameters.have_directional_light);
		create_or_update(uniforms, "vr_directional_light.direction", glm::vec3{ 0.f, 0.f, 0.f } - parameters.directional_light.position);
		create_or_update(uniforms, "vr_directional_light.components.ambient", parameters.directional_light.components.ambient);
		create_or_update(uniforms, "vr_directional_light.components.diffuse", parameters.directional_light.components.diffuse);
		create_or_update(uniforms, "vr_directional_light.components.specular", parameters.directional_light.components.specular);

		create_or_update(uniforms, "vr_have_point_light", parameters.have_point_light);
		create_or_update(uniforms, "vr_point_light.position", parameters.point_light.position);
		create_or_update(uniforms, "vr_point_light.components.ambient", parameters.point_light.components.ambient);
		create_or_update(uniforms, "vr_point_light.components.diffuse", parameters.point_light.components.diffuse);
		create_or_update(uniforms, "vr_point_light.components.specular", parameters.point_light.components.specular);
		create_or_update(uniforms, "vr_point_light.attenuation.constant", parameters.point_light.attenuation.constant);
		create_or_update(uniforms, "vr_point_light.attenuation.linear", parameters.point_light.attenuation.linear);
		create_or_update(uniforms, "vr_point_light.attenuation.quadratic", parameters.point_light.attenuation.quadratic);

		create_or_update(uniforms, "vr_have_spot_light", parameters.have_spot_light);
		create_or_update(uniforms, "vr_spot_light.position", parameters.spot_light.position);
		create_or_update(uniforms, "vr_spot_light.direction", target - parameters.spot_light.position);
		create_or_update(uniforms, "vr_spot_light.cutoff_cosine", glm::cos(glm::radians(parameters.spot_light.cutoff_angle)));
		create_or_update(uniforms, "vr_spot_light.outer_cutoff_cosine", glm::cos(glm::radians(parameters.spot_light.outer_cutoff_angle)));
		create_or_update(uniforms, "vr_spot_light.components.ambient", parameters.spot_light.components.ambient);
		create_or_update(uniforms, "vr_spot_light.components.diffuse", parameters.spot_light.components.diffuse);
		create_or_update(uniforms, "vr_spot_light.components.specular", parameters.spot_light.components.specular);
		create_or_update(uniforms, "vr_spot_light.attenuation.constant", parameters.spot_light.attenuation.constant);
		create_or_update(uniforms, "vr_spot_light.attenuation.linear", parameters.spot_light.attenuation.linear);
		create_or_update(uniforms, "vr_spot_light.attenuation.quadratic", parameters.spot_light.attenuation.quadratic);
	}
}

void main_loop::transform_model(const parameters& parameters)
{
	assert(m_directional_light);
	assert(m_point_light);
	assert(m_spot_light);

	m_directional_light->set_translation(parameters.directional_light.position);
	m_point_light->set_translation(parameters.point_light.position);
	m_spot_light->set_translation(parameters.spot_light.position);

	if (m_scene_model)
	{
		auto get_world_position = [](vr::object3d* object) -> glm::vec3
		{
			glm::vec3 scale;
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(object->get_transformation_matrix(), scale, rotation, translation, skew, perspective);
			return translation;
		};

		m_scene_model->traverse([&, this](vr::object3d* obj)
			{
				const auto object_position_world = get_world_position(m_scene_model);
				set_light_parameters(obj, parameters, object_position_world);
			});

		m_scene_model->set_translation(parameters.translation.vec);

		m_scene_model->set_rotation(vr::x_axis, parameters.rotation.euler_angles.x);
		m_scene_model->rotate_world(vr::y_axis, parameters.rotation.euler_angles.y);
		m_scene_model->rotate_world(vr::z_axis, parameters.rotation.euler_angles.z);

		m_scene_model->set_scale({ parameters.scale.value.x, parameters.scale.value.y, parameters.scale.value.z });
	}
}