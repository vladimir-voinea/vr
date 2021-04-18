#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include "shader_loader.hpp"
#include "texture_loader.hpp"

#include <cube_texture_material.hpp>

#include <glfw_util.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <spdlog/spdlog.h>

#include <stdexcept>

static const auto start_position = glm::vec3{ -2.7872, 1.74459, 9.47206 };
static const auto start_direction = glm::vec3{ 0.115009114, 0.016061125, -0.9932346 };

vr::perspective_camera::settings make_camera_settings(vr::glfw::window& window) 
{
	vr::perspective_camera::settings settings { 45.f, static_cast<float>(window.get_viewport_size().width) / static_cast<float>(window.get_viewport_size().height), 0.1f, 100.f,
	/*start_position, start_direction*/ };

	return settings;
}

main_loop::main_loop(vr::glfw::window& window)
	: m_window(window)
	, m_kb(window)
	, m_mouse(window)
	, m_camera_settings(make_camera_settings(m_window))
	, m_camera(std::make_unique<vr::perspective_camera>(m_camera_settings))
	, m_fps_counter(vr::glfw::get_time())
	, m_input_listener(m_window, *m_camera, m_fps_counter)
	//, m_controls(m_window, *m_camera, m_mouse, m_kb)
{
	init();
}

main_loop::~main_loop()
{
}

void main_loop::initialize_controls()
{
	m_kb.set_listener(&m_input_listener);
	m_mouse.set_listener(&m_input_listener);
	m_mouse.set_mode(vr::glfw::mouse_mode::disabled);
	const auto viewport_size = m_window.get_viewport_size();
	const auto mouse_position = vr::glfw::mouse_position{ static_cast<double>(viewport_size.width / 2), static_cast<double>(viewport_size.height / 2) };
	m_mouse.set_position(mouse_position);

	m_kb.set_sticky_keys(true);
	m_mouse.set_sticky_buttons(true);
}

void main_loop::initialize_position()
{
	//m_camera->set_position({ -2.7872, 1.74459, 9.47206 });
	//m_camera->set_direction({ 0.115009114, 0.016061125, -0.9932346 });
}

void main_loop::process_input()
{
	//m_controls.process_events(m_delta_time);

	if (m_kb.get_key_state(vr::glfw::key::escape) == vr::glfw::key_action::press)
	{
		m_window.request_close();
	}

}

void main_loop::print_state()
{
	//spdlog::info("Position: {0}, {1}, {2}", m_camera->get_position().x, m_camera->get_position().y, m_camera->get_position().z);
	//spdlog::info("Direction: {0}, {1}, {2}", m_camera->get_direction().x, m_camera->get_direction().y, m_camera->get_direction().z);
	//spdlog::info("FPS: {0}", m_fps_counter->get_fps());

	//for (auto i = 0u; i < m_monkeys.size(); ++i)
	//{
	//	std::cout << "Monkey " << i << " opos: " << m_monkeys[i].obj->get_translation().x << ' ' << m_monkeys[i].obj->get_translation().y << ' ' << m_monkeys[i].obj->get_translation().z << '\n';
	//	std::cout << "Monkey " << i << " lpos: " << m_monkeys[i].uniforms->at(2).value.vec3f.x << ' ' << m_monkeys[i].uniforms->at(2).value.vec3f.y << ' ' << m_monkeys[i].uniforms->at(2).value.vec3f.z << '\n';
	//}
}

vr::geometry import_model(const std::string& name)
{
	const auto path = "data/models/" + name + ".obj";

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiPostProcessSteps::aiProcess_ValidateDataStructure |
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

void main_loop::init()
{
	m_cube_texture = std::make_unique<vr::cube_texture>(std::unordered_map<std::string, std::string>{
		//{ vr::cube_texture::p_x, "data/skybox/right.jpg" },
		//{ vr::cube_texture::n_x, "data/skybox/left.jpg" },
		//{ vr::cube_texture::p_y, "data/skybox/top.jpg" },
		//{ vr::cube_texture::n_y, "data/skybox/bottom.jpg" },
		//{ vr::cube_texture::n_z, "data/skybox/back.jpg" },
		//{ vr::cube_texture::p_z, "data/skybox/front.jpg" }
		{ vr::cube_texture::p_x, "data/skybox/urban-skyboxes/SaintLazarusChurch2/posx.jpg" },
		{ vr::cube_texture::n_x, "data/skybox/urban-skyboxes/SaintLazarusChurch2/negx.jpg" },
		{ vr::cube_texture::p_y, "data/skybox/urban-skyboxes/SaintLazarusChurch2/posy.jpg" },
		{ vr::cube_texture::n_y, "data/skybox/urban-skyboxes/SaintLazarusChurch2/negy.jpg" },
		{ vr::cube_texture::n_z, "data/skybox/urban-skyboxes/SaintLazarusChurch2/negz.jpg" },
		{ vr::cube_texture::p_z, "data/skybox/urban-skyboxes/SaintLazarusChurch2/posz.jpg" }
	});
	m_skybox_material = std::make_unique<vr::gl::cube_texture_material>();

	m_renderer_settings.skybox = std::make_unique<vr::skybox>(m_skybox_material.get(), m_cube_texture.get());
	//m_renderer_settings.clear_color = std::make_unique<glm::vec3>(66, 155, 245);

	m_renderer_settings.cull_faces = true;
	m_renderer_settings.wireframe_mode = false;
	m_renderer_settings.size.width = m_window.get_viewport_size().width;
	m_renderer_settings.size.height = m_window.get_viewport_size().height;

	m_renderer = std::make_unique<vr::gl::renderer>(m_renderer_settings);
	m_window.set_framebuffer_resize_callback([this](int width, int height)
		{
			spdlog::info("New framebuffer size: {0}, {1}", width, height);
			m_camera_settings.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
			m_renderer_settings.size.width = width;
			m_renderer_settings.size.height = height;
		});

	initialize_controls();
	initialize_position();

	std::random_device dev;
	m_random_engine = std::default_random_engine(dev());

	{
		m_monkey_data.geometry = ::import_model("suzanne");
		m_monkey_data.shader = std::make_unique<vr::gl::opengl_shader>(load_vertex_shader_code("suzanne"), load_fragment_shader_code("suzanne"));
		m_monkey_data.texture_uvmap = std::make_unique<vr::texture>("data/models/uvmap.DDS");
		m_monkey_data.texture_cobblestone = std::make_unique<vr::texture>("data/models/light_bricks.jpg");
		m_monkey_data.material = std::make_unique<vr::gl::color_material>(glm::vec4{ 255, 0, 0, 0 });
	}

	std::uniform_real_distribution<> limits_n(-5.f, 0.f);
	std::uniform_real_distribution<> limits_p(0.f, 5.f);

	for (auto i = 0u; i < n_monkeys; ++i)
	{
		monkey_instance inst;
		inst.obj = std::make_unique<vr::object3d>();

		inst.material = std::make_unique<vr::gl::opengl_shader_material>(*m_monkey_data.shader, inst.uniforms.get());
		vr::texture* texture = m_monkey_data.texture_uvmap.get();
		inst.mesh = std::make_unique<vr::mesh>(&m_monkey_data.geometry, inst.material.get(), texture);

		inst.obj->add_mesh(inst.mesh.get());

		inst.x_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));
		inst.y_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));
		inst.z_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));

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

void main_loop::render_scene()
{
	const auto current_time = static_cast<float>(vr::glfw::get_time());
	m_delta_time = current_time - m_last_timestamp;
	m_last_timestamp = current_time;

	m_fps_counter.frame(current_time);

	const auto projection_matrix = m_camera->get_projection_matrix();
	const auto view_matrix = m_camera->get_view_matrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::bernoulli_distribution true_or_false;
	auto p_or_n = [&, true_or_false, this]() -> int { return  true_or_false(m_random_engine) ? 1 : -1; };

	std::uniform_int_distribution distance_rand(1, 20);
	int light_distance_from_object = distance_rand(m_random_engine);
	auto light_direction_from_object = glm::vec3(p_or_n() * light_distance_from_object, p_or_n() * light_distance_from_object, p_or_n() * light_distance_from_object);

	std::uniform_int_distribution axis_picker_rand(1, 3);
	auto pick_axis = [&, axis_picker_rand, this]()
	{
		const auto random = axis_picker_rand(m_random_engine);
		switch (random)
		{
		case 1:
		{
			return vr::x_axis;
		}
		case 2:
		{
			return vr::y_axis;
		}
		case 3:
			return vr::z_axis;
		}
	};

	std::uniform_real_distribution zero_one_rand(0.f, 1.f);
	auto zero_one_vec = [&, zero_one_rand, this]()
	{
		return glm::normalize(glm::vec3(zero_one_rand(m_random_engine), zero_one_rand(m_random_engine), zero_one_rand(m_random_engine)));
	};

	const auto rotation_angle = 2.f;
	m_monkeys.front().obj->rotate(vr::y_axis, rotation_angle * m_delta_time);

	m_renderer->render(m_scene, *m_camera);

	m_window.swap_buffers();
}

void main_loop::run()
{
	while (!m_window.close_requested())
	{
		if (m_window.has_focus())
		{
			process_input();
			print_state();
			render_scene();
		}

		vr::glfw::poll_events();
	}
}