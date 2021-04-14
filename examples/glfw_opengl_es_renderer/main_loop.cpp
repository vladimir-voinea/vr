#include "main_loop.hpp"
#include <perspective_camera.hpp>
#include "cube_data.hpp"
#include "opengl_debug_callback.hpp"
#include "shader_loader.hpp"
#include "texture_loader.hpp"

#include <glfw_util.hpp>

#include <vr-opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stdexcept>
#include <iostream>

void initialize_glew()
{
	glewExperimental = GL_TRUE;
	const auto glew_initialization = glewInit();
	if (glew_initialization != GLEW_OK)
	{
		throw std::runtime_error("Could not initialize glew");
	}
}

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
	delete m_monkey_data.obj;
	delete m_monkey_data.mesh;
	delete m_monkey_data.material;
	delete m_monkey_data.texture;
}

void main_loop::initialize_controls()
{
	m_kb.set_sticky_keys(true);
	m_mouse.set_sticky_buttons(true);
	m_mouse.set_mode(vr::glfw::mouse::mode::disabled);
}

void main_loop::initialize_position()
{
	m_camera->set_position({ -8.96424, 2.70909, 4.2585 });
	m_camera->set_direction({ 0.827756, -0.307191, -0.469526 });
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
	std::cout << "Position: "
		<< m_camera->get_position().x << ", "
		<< m_camera->get_position().y << ", "
		<< m_camera->get_position().z << '\n';

	std::cout << "Direction: " <<
		m_camera->get_direction().x << ", "
		<< m_camera->get_direction().y << ", "
		<< m_camera->get_direction().z << '\n';
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
	initialize_glew();
	initialize_controls();
	initialize_position();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(opengl_debug_callback, nullptr);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	{
		m_monkey_data.geometry = ::import_model("suzanne");

		vr::gl::uniform v, m, light_position;
		v.name = "v";
		v.type = vr::gl::uniform_type::mat4fv;
		v.value.mat4fv = glm::mat4(1.0f);

		m.name = "m";
		m.type = vr::gl::uniform_type::mat4fv;
		m.value.mat4fv = glm::mat4(1.0f);

		light_position.name = "light_position_world";
		light_position.type = vr::gl::uniform_type::vec3f;
		light_position.value.vec3f = glm::vec3(4, 4, 4);

		m_monkey_data.uniforms.push_back(v);
		m_monkey_data.uniforms.push_back(m);
		m_monkey_data.uniforms.push_back(light_position);

		m_monkey_data.shader = new vr::gl::opengl_shader{ load_vertex_shader_code("suzanne"), load_fragment_shader_code("suzanne") };
		m_monkey_data.material = new vr::gl::opengl_shader_material(*m_monkey_data.shader, m_monkey_data.uniforms);
		m_monkey_data.texture = new vr::texture("data/models/uvmap.DDS");
		
		m_monkey_data.mesh = new vr::mesh{ &m_monkey_data.geometry, m_monkey_data.material, m_monkey_data.texture };

		m_monkey_data.obj = new vr::object3d{};
		m_monkey_data.obj->add_mesh(m_monkey_data.mesh);

		m_scene.add(m_monkey_data.obj);
	}

	m_last_timestamp = vr::glfw::get_time();
}


void main_loop::render_scene()
{
	const auto current_time = static_cast<float>(vr::glfw::get_time());
	m_delta_time = current_time - m_last_timestamp;
	m_last_timestamp = current_time;

	const auto projection_matrix = m_camera->get_projection_matrix();
	const auto view_matrix = m_camera->get_view_matrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		auto mk = m_monkey_data.obj;
		mk->set_position(mk->get_position() + glm::vec3(0.5, 0.0, 0.0) * m_delta_time);
		
		m_monkey_data.uniforms[0].value.mat4fv = view_matrix;
		m_monkey_data.uniforms[1].value.mat4fv = m_monkey_data.obj->get_transformation_matrix();

		m_renderer.render(m_scene, *m_camera);
	}



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