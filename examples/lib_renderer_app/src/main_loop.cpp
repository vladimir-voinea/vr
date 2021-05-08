#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include <orthographic_camera.hpp>
#include "shader_loader.hpp"
#include "shaders.hpp"

#include <cube_texture_material.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>
#include <stdexcept>

#include <android_logging.hpp>

static const auto start_position = glm::vec3{ -2.7872, 1.74459, 9.47206 };
static const auto start_direction = glm::vec3{ 0.115009114, 0.016061125, -0.9932346 };

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

	m_scene_model = vr::model::load_model("model.dae");
	//m_monkey_model.root_node->scale({ 0.01, 0.01, 0.01 });
	//m_scene_model.root_node->rotate(vr::x_axis, -90);
	m_scene.add(m_scene_model.root_node.get());
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

void main_loop::frame(float delta_time)
{
	const auto rotation_angle = -114.f;
	//m_scene_model.root_node->rotate(vr::y_axis, rotation_angle * delta_time);

	m_renderer->render(m_scene, get_camera());
}