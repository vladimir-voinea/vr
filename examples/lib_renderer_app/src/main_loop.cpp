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

	m_renderer_settings.skybox = std::make_unique<vr::skybox>(m_skybox_material.get(), m_cube_texture.get());

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
	m_directional_light_object = m_directional_light_model.first.get();
	m_scene.add_child(std::move(m_directional_light_model.first));

	m_point_light_model = vr::model::load_model("data/models/torus.obj");
	m_point_light_object = m_point_light_model.first.get();
	m_scene.add_child(std::move(m_point_light_model.first));

	m_spot_light_model = vr::model::load_model("data/models/cone.obj");
	m_spot_light_object = m_spot_light_model.first.get();
	m_scene.add_child(std::move(m_spot_light_model.first));
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
				m_loaded_models[m_scene_model_path].first = std::move(m_scene.remove_child(m_scene_model));
			}

			auto it = m_loaded_models.find(parameters.path);
			if (it == m_loaded_models.end())
			{
				auto insert = m_loaded_models.insert(std::make_pair(parameters.path, vr::model::load_model(parameters.path)));
				it = insert.first;
			}

			m_scene_model = it->second.first.get();
			m_scene_model->set_name("Model root");
			m_scene.add_child(std::move(it->second.first));

			m_scene_model_path = parameters.path;
		}
	}

	setup_lights(parameters);
	transform_model(parameters);

	m_renderer->render(m_scene, get_camera());
}

void main_loop::setup_lights(const parameters& parameters)
{
	auto make_components = [](const light_components& components)
	{
		vr::light_components result;
		result.ambient = components.ambient;
		result.diffuse = components.diffuse;
		result.specular = components.specular;
		return result;
	};

	auto make_attenuation = [](const light_attenuation& attenuation)
	{
		vr::light_attenuation result;
		result.constant = attenuation.constant;
		result.linear = attenuation.linear;
		result.quadratic = attenuation.quadratic;
		return result;
	};

	if (parameters.have_ambient_light)
	{
		const auto scene_has_light_light = m_ambient_light;
		m_scene.traverse([&, this](auto obj)
			{
				if (obj->get_name() == "Ambient light")
				{
					m_ambient_light = static_cast<vr::ambient_light*>(obj);
				}
			});

		if (!m_ambient_light)
		{
			auto a_l = std::make_unique<vr::ambient_light>(parameters.ambient_light.color, parameters.ambient_light.intensity);
			m_ambient_light = a_l.get();
			m_ambient_light->set_name("Ambient light");

			m_scene.add_child(std::move(a_l));
		}

		if (!scene_has_light_light)
		{
			m_scene.get_lights().add(m_ambient_light);
		}

		m_ambient_light->set_color(parameters.ambient_light.color);
		m_ambient_light->set_intensity(parameters.ambient_light.intensity);
	}
	else
	{
		if (m_ambient_light)
		{
			m_scene.get_lights().remove(m_ambient_light);
			m_ambient_light = nullptr;
		}
	}

	m_directional_light_object->set_translation(parameters.directional_light.position);
	if (parameters.have_directional_light)
	{
		const auto scene_has_light_light = m_directional_light;
		m_scene.traverse([&, this](auto obj)
			{
				if (obj->get_name() == "Directional light")
				{
					m_directional_light = static_cast<vr::directional_light*>(obj);
				}
			});

		if (!m_directional_light)
		{
			auto d_l = std::make_unique<vr::directional_light>(make_components(parameters.directional_light.components), parameters.directional_light.intensity);
			m_directional_light = d_l.get();
			m_directional_light->set_name("Directional light");

			m_directional_light_object->add_child(std::move(d_l));
		}

		if (!scene_has_light_light)
		{
			m_scene.get_lights().add(m_directional_light);
		}

		m_directional_light->set_components(make_components(parameters.directional_light.components));
		m_directional_light->set_intensity(parameters.directional_light.intensity);
	}
	else
	{
		if (m_directional_light)
		{
			m_scene.get_lights().remove(m_directional_light);
			m_directional_light = nullptr;
		}
	}

	m_point_light_object->set_translation(parameters.point_light.position);
	if (parameters.have_point_light)
	{
		const auto scene_has_light_light = m_point_light;
		m_scene.traverse([&, this](auto obj)
			{
				if (obj->get_name() == "Point light")
				{
					m_point_light = static_cast<vr::point_light*>(obj);
				}
			});

		if (!m_point_light)
		{
			auto p_l = std::make_unique<vr::point_light>(make_components(parameters.point_light.components), make_attenuation(parameters.point_light.attenuation), parameters.point_light.intensity);
			m_point_light = p_l.get();
			m_point_light->set_name("Point light");

			m_point_light_object->add_child(std::move(p_l));
		}

		if (!scene_has_light_light)
		{
			m_scene.get_lights().add(m_point_light);
		}

		m_point_light->set_components(make_components(parameters.point_light.components));
		m_point_light->set_attenuation(make_attenuation(parameters.point_light.attenuation));
		m_point_light->set_intensity(parameters.point_light.intensity);
	}
	else
	{
		if (m_point_light)
		{
			m_scene.get_lights().remove(m_point_light);
			m_point_light = nullptr;
		}
	}

	m_spot_light_object->set_translation(parameters.spot_light.position);
	if (parameters.have_spot_light)
	{
		const auto scene_has_light_light = m_spot_light;
		m_scene.traverse([&, this](auto obj)
			{
				if (obj->get_name() == "Spot light")
				{
					m_spot_light = static_cast<vr::spot_light*>(obj);
				}
			});

		if (!m_spot_light)
		{
			auto s_l = std::make_unique<vr::spot_light>(make_components(parameters.point_light.components), make_attenuation(parameters.point_light.attenuation), 
				parameters.spot_light.cutoff_angle, parameters.spot_light.outer_cutoff_angle, parameters.spot_light.intensity);
			m_spot_light = s_l.get();
			m_spot_light->set_name("Spot light");

			m_spot_light_object->add_child(std::move(s_l));
		}

		if (!scene_has_light_light)
		{
			m_scene.get_lights().add(m_spot_light);
		}

		m_spot_light->set_components(make_components(parameters.spot_light.components));
		m_spot_light->set_attenuation(make_attenuation(parameters.spot_light.attenuation));
		m_spot_light->set_inner_cutoff_angle(parameters.spot_light.cutoff_angle);
		m_spot_light->set_outer_cutoff_angle(parameters.spot_light.outer_cutoff_angle);
		m_spot_light->set_intensity(parameters.spot_light.intensity);
	}
	else
	{
		if (m_spot_light)
		{
			m_scene.get_lights().remove(m_spot_light);
			m_spot_light = nullptr;
		}
	}
}

void main_loop::transform_model(const parameters& parameters)
{
	if (m_scene_model)
	{
		m_scene_model->set_translation(parameters.translation.vec);

		m_scene_model->set_rotation(vr::x_axis, parameters.rotation.euler_angles.x);
		m_scene_model->rotate_world(vr::y_axis, parameters.rotation.euler_angles.y);
		m_scene_model->rotate_world(vr::z_axis, parameters.rotation.euler_angles.z);

		m_scene_model->set_scale({ parameters.scale.value.x, parameters.scale.value.y, parameters.scale.value.z });
	}
}