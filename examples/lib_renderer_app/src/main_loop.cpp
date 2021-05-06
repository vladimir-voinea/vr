#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include <orthographic_camera.hpp>
#include "shader_loader.hpp"
#include "shaders.hpp"

#include <cube_texture_material.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <platform_manager_factory.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <spdlog/spdlog.h>
#include <stdexcept>

#include <android_logging.hpp>

vr::geometry import_model(const std::string& asset_name)
{
	const auto name = "models/" + asset_name + ".dae";

	auto am = vr::platform::get_platform_manager()->get_asset_manager();
	auto asset = am->get_asset_by_name(name);
	const auto file = am->read_file(asset);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFileFromMemory(file.data(), file.size(), aiPostProcessSteps::aiProcess_ValidateDataStructure |
		aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
		aiPostProcessSteps::aiProcess_Triangulate);
	if (!scene)
	{
		const std::string message = "Could not import model: " + std::string(importer.GetErrorString());
		throw std::runtime_error(message);
	}

	const aiMesh* mesh = scene->mMeshes[0];

	vr::geometry geometry;

	{
		geometry.attributes["vr_vertex_position"].components = 3;
		geometry.attributes["vr_vertex_position"].type = vr::attribute::data_type::t_float;
		const auto begin = reinterpret_cast<const uint8_t*>(mesh->mVertices);
		const auto end = reinterpret_cast<const uint8_t*>(&mesh->mVertices[mesh->mNumVertices]);
		std::copy(begin, end, std::back_inserter(geometry.attributes["vr_vertex_position"].data));
	}

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		geometry.indices.push_back(mesh->mFaces[i].mIndices[0]);
		geometry.indices.push_back(mesh->mFaces[i].mIndices[1]);
		geometry.indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	if (mesh->HasNormals())
	{
		geometry.attributes["vr_vertex_normal"].components = 3;
		geometry.attributes["vr_vertex_normal"].type = vr::attribute::data_type::t_float;
		const auto begin = reinterpret_cast<const uint8_t*>(mesh->mNormals);
		std::copy(begin, begin + mesh->mNumVertices * sizeof(decltype(*mesh->mNormals)), std::back_inserter(geometry.attributes["vr_vertex_normal"].data));
	}

	if (mesh->HasVertexColors(0))
	{
		geometry.attributes["vr_vertex_color"].components = 4;
		geometry.attributes["vr_vertex_color"].type = vr::attribute::data_type::t_float;
		const auto begin = reinterpret_cast<const uint8_t*>(mesh->mColors);
		std::copy(begin, begin + mesh->mNumVertices + sizeof(decltype(*mesh->mColors)), std::back_inserter(geometry.attributes["vr_vertex_color"].data));
	}

	if (mesh->HasTextureCoords(0))
	{
		geometry.attributes["vr_vertex_uv"].type = vr::attribute::data_type::t_float;
		geometry.attributes["vr_vertex_uv"].components = 2;
		for (auto i = 0u; i < mesh->mNumVertices; ++i)
		{
			const auto uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			const auto begin = reinterpret_cast<const uint8_t*>(&uv);
			std::copy(begin, begin + sizeof(uv), std::back_inserter(geometry.attributes["vr_vertex_uv"].data));
		}
	}

	return geometry;
}

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

	std::random_device dev;

	{
		m_monkey_data.geometry = ::import_model("suzanne");
		m_monkey_data.shader = std::make_unique<vr::gl::opengl_shader>(load_vertex_shader_code("suzanne"), load_fragment_shader_code("suzanne"));
		m_monkey_data.texture_uvmap = std::make_unique<vr::texture>("models/uvmap.png");
	}

	std::uniform_real_distribution<> limits_n(-5.f, 0.f);
	std::uniform_real_distribution<> limits_p(0.f, 5.f);

	for (auto i = 0u; i < n_monkeys; ++i)
	{
		monkey inst;
		inst.obj = std::make_unique<vr::object3d>();

		inst.material = std::make_unique<vr::gl::opengl_shader_material>(*m_monkey_data.shader, inst.uniforms.get());
		vr::texture* texture = m_monkey_data.texture_uvmap.get();
		inst.mesh = std::make_unique<vr::mesh>(&m_monkey_data.geometry, inst.material.get(), texture);

		inst.obj->add_mesh(inst.mesh.get());

		m_monkeys.push_back(std::move(inst));
	}
	m_scene.add(m_monkeys[0].obj.get());
	m_monkeys[0].obj->translate(glm::vec3(0.f, 10.f, -10.f));

	float offset = 5.f;
	auto mid = m_monkeys.size() / 2;

	for (auto i = 1u; i <= mid; ++i)
	{
		const auto previous = i - 1;
		m_monkeys[previous].obj->add_child(m_monkeys[i].obj.get());
		m_monkeys[i].obj->set_parent(m_monkeys[previous].obj.get());
		m_monkeys[i].obj->translate(glm::vec3(-offset, -offset, 0.f));
	}

	m_monkeys[0].obj->add_child(m_monkeys[mid + 1].obj.get());
	m_monkeys[mid + 1].obj->set_parent(m_monkeys[0].obj.get());
	m_monkeys[mid + 1].obj->translate(glm::vec3(offset, -offset, 0.f));

	for (auto i = mid + 2; i <= 2 * mid; ++i)
	{
		const auto previous = i - 1;

		m_monkeys[previous].obj->add_child(m_monkeys[i].obj.get());
		m_monkeys[i].obj->set_parent(m_monkeys[previous].obj.get());
		m_monkeys[i].obj->translate(glm::vec3(offset, -offset, 0.f));
	}

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
	const auto rotation_angle = -2.f;
	m_monkeys.front().obj->rotate(vr::y_axis, rotation_angle * delta_time);

	m_renderer->render(m_scene, get_camera());
}