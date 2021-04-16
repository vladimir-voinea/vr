#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include "cube_data.hpp"
#include "opengl_debug_callback.hpp"
#include "shader_loader.hpp"
#include "texture_loader.hpp"

#include <glfw_util.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stdexcept>
#include <iostream>


main_loop::main_loop(vr::glfw::window& window)
	: m_window(window)
	, m_kb(window)
	, m_mouse(window)
	, m_camera(std::make_unique<vr::perspective_camera>(vr::perspective_camera::settings{
	45.f, static_cast<float>(window.get_size().width) / static_cast<float>(window.get_size().height),
	0.1f,100.f }))
	, m_controls(m_window, *m_camera, m_mouse, m_kb)
{
	init();
}

main_loop::~main_loop()
{
}

void main_loop::initialize_controls()
{
	m_kb.set_sticky_keys(true);
	m_mouse.set_sticky_buttons(true);
	m_mouse.set_mode(vr::glfw::mouse::mode::disabled);
}

void main_loop::initialize_position()
{
	m_camera->set_position({ -2.7872, 1.74459, 9.47206 });
	m_camera->set_direction({ 0.f, -0.231316, 0.f });
}

void main_loop::process_input()
{
	m_controls.process_events(m_delta_time);

	if (m_kb.get_key_state(vr::glfw::keyboard::key::escape) == vr::glfw::keyboard::state::press)
	{
		m_window.request_close();
	}

}

void main_loop::print_state()
{
	//std::cout << "Position: "
	//	<< m_camera->get_position().x << ", "
	//	<< m_camera->get_position().y << ", "
	//	<< m_camera->get_position().z << '\n';

	//std::cout << "Direction: " <<
	//	m_camera->get_direction().x << ", "
	//	<< m_camera->get_direction().y << ", "
	//	<< m_camera->get_direction().z << '\n';

	std::cout << "FPS: " << m_fps_counter->get_fps() << '\n';
	//
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
	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		vr::vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		geometry.vertices.push_back(vertex);
	}

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		geometry.indices.push_back(mesh->mFaces[i].mIndices[0]);
		geometry.indices.push_back(mesh->mFaces[i].mIndices[1]);
		geometry.indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	return geometry;
}

void main_loop::init()
{
	m_renderer_settings.clear_color = glm::vec3(66, 155, 245);
	m_renderer_settings.cull_faces = true;
	m_renderer_settings.wireframe_mode = false;

	m_renderer = std::make_unique<vr::gl::renderer>(m_renderer_settings);
	
	initialize_controls();
	initialize_position();

	std::random_device dev;
	m_random_engine = std::default_random_engine(dev());

	{
		m_monkey_data.geometry = ::import_model("textured_cube");
		m_monkey_data.shader = std::make_unique<vr::gl::opengl_shader>(load_vertex_shader_code("suzanne"), load_fragment_shader_code("suzanne"));
		m_monkey_data.texture_uvmap = std::make_unique<vr::texture>("data/models/uvmap.DDS");
		m_monkey_data.texture_cobblestone = std::make_unique<vr::texture>("data/models/light_bricks.jpg");
	}

	std::uniform_real_distribution<> limits_n(-5.f, 0.f);
	std::uniform_real_distribution<> limits_p(0.f, 5.f);

	for (auto i = 0u; i < n_monkeys; ++i)
	{
		monkey_instance inst;
		inst.obj = std::make_unique<vr::object3d>();

		inst.material = std::make_unique<vr::gl::opengl_shader_material>(*m_monkey_data.shader, inst.uniforms.get());
		inst.mesh = std::make_unique<vr::mesh>(&m_monkey_data.geometry, inst.material.get(), i ? m_monkey_data.texture_uvmap.get() : m_monkey_data.texture_cobblestone.get());

		inst.obj->add_mesh(inst.mesh.get());

		inst.x_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));
		inst.y_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));
		inst.z_rand = std::uniform_real_distribution<>(limits_n(m_random_engine), limits_p(m_random_engine));

		vr::object3d* previous_monkey = !m_monkeys.empty() ? m_monkeys.back().obj.get() : nullptr;
		m_monkeys.push_back(std::move(inst));
		vr::object3d* last_added_monkey = m_monkeys.back().obj.get();
		if (!i)
		{
			m_scene.add(m_monkeys.back().obj.get());
		}
		else
		{
			previous_monkey->add_child(last_added_monkey);
			last_added_monkey->set_parent(previous_monkey);
			last_added_monkey->translate(previous_monkey->get_translation() + glm::vec3(10.f, 0.f, 0.f));
		}

	}

	m_last_timestamp = static_cast<float>(vr::glfw::get_time());
	m_fps_counter = std::make_unique<fps_counter>(m_last_timestamp);
}


void main_loop::render_scene()
{
	const auto current_time = static_cast<float>(vr::glfw::get_time());
	m_delta_time = current_time - m_last_timestamp;
	m_last_timestamp = current_time;

	m_fps_counter->frame(current_time);

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

	std::uniform_real_distribution axis_rand(0.f, 1.f);
	auto random_axis = [&, axis_rand, this]()
	{
		return glm::normalize(glm::vec3(axis_rand(m_random_engine), axis_rand(m_random_engine), axis_rand(m_random_engine)));
	};

//	m_monkeys.front().obj->rotate(vr::z_axis, 2.f * m_delta_time);

	//for (auto i = 0; i < m_monkeys.size(); ++i)
	//{
	//	auto& monkey = m_monkeys[i];
	//	
	//	if (!i) {
	//		const auto rotation_angle = p_or_n() * 2.f;
	//		monkey.obj->rotate(vr::z_axis, rotation_angle * m_delta_time);
	//	}

		//if (i) {
		//	auto new_position = glm::vec3(monkey.x_rand(m_random_engine), monkey.y_rand(m_random_engine), monkey.z_rand(m_random_engine));
		//	monkey.obj->translate(monkey.obj->get_translation() + new_position * m_delta_time);
		//}
	//}

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