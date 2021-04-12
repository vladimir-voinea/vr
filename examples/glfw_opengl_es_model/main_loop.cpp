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
	, m_camera(std::make_unique<vr::gl::perspective_camera>(vr::gl::perspective_camera::settings{
	45.f, static_cast<float>(window.get_size().width) / static_cast<float>(window.get_size().height),
	0.1f,100.f }))
	, m_controls(m_window, *m_camera, m_mouse, m_kb)
{
	init();
}

main_loop::~main_loop()
{
	glDeleteBuffers(1, &m_suzanne_vertex_buffer);
	glDeleteBuffers(1, &m_suzanne_uv_buffer);
	glDeleteBuffers(1, &m_suzanne_normal_buffer);
	glDeleteTextures(1, &m_suzanne_texture);
	glDeleteVertexArrays(1, &m_suzanne_vertex_array);

	glDeleteBuffers(1, &m_cube_color_buffer);
	glDeleteBuffers(1, &m_cube_vertex_buffer);
	glDeleteVertexArrays(1, &m_cube_vertex_array);
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
	const auto current_time = vr::glfw::get_time();
	const float delta_time = static_cast<float>(current_time - m_last_timestamp);

	m_controls.process_events(delta_time);

	if (m_kb.get_key_state(vr::glfw::keyboard::key::escape) == vr::glfw::keyboard::state::press)
	{
		m_window.request_close();
	}

	m_last_timestamp = current_time;
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
		// load cube
		m_cube_shaders = load_shaders("cube", "cube");
		m_cube_mvp_uniform = glGetUniformLocation(m_cube_shaders.program.get_id(), "mvp");

		glGenVertexArrays(1, &m_cube_vertex_array);
		glBindVertexArray(m_cube_vertex_array);

		glGenBuffers(1, &m_cube_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_vertex_buffer);
		constexpr auto vertex_data_size = sizeof(vertex_data);
		glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

		glGenBuffers(1, &m_cube_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_color_buffer);
		constexpr auto color_data_size = sizeof(color_data);
		glBufferData(GL_ARRAY_BUFFER, color_data_size, color_data, GL_STATIC_DRAW);
	}

	{
		// load suzanne
		const auto model = import_model();

		m_suzanne_texture = load_dds("data/models/uvmap.DDS");
		m_suzanne_shaders = load_shaders("suzanne", "suzanne");
		m_suzanne_mvp_uniform = glGetUniformLocation(m_suzanne_shaders.program.get_id(), "mvp");
		m_suzanne_view_matrix_uniform = glGetUniformLocation(m_suzanne_shaders.program.get_id(), "v");
		m_suzanne_model_matrix_uniform = glGetUniformLocation(m_suzanne_shaders.program.get_id(), "m");
		m_suzanne_light_position_world_uniform = glGetUniformLocation(m_suzanne_shaders.program.get_id(), "light_position_world");
		m_suzanne_texture_shader_sampler = glGetUniformLocation(m_suzanne_shaders.program.get_id(), "texture_sampler");
	
		glGenVertexArrays(1, &m_suzanne_vertex_array);
		glBindVertexArray(m_suzanne_vertex_array);

		glGenBuffers(1, &m_suzanne_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_vertex_buffer);
		const auto size = model.vertices.size() * sizeof(decltype(model.vertices)::value_type);
		glBufferData(GL_ARRAY_BUFFER, size, model.vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_suzanne_uv_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, model.uvs.size() * sizeof(decltype(model.uvs)::value_type), model.uvs.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_suzanne_normal_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_normal_buffer);
		glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(decltype(model.normals)::value_type), model.normals.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_suzanne_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_suzanne_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(decltype(model.indices)::value_type), model.indices.data(), GL_STATIC_DRAW);

		m_suzanne_indices = model.indices.size();
	}

	m_last_timestamp = vr::glfw::get_time();
}

main_loop::suzanne_geometry main_loop::import_model()
{
	constexpr auto path = "data/models/suzanne.obj";
	
	Assimp::Importer importer;
	const aiScene* suzanne = importer.ReadFile(path, aiPostProcessSteps::aiProcess_ValidateDataStructure);

	if (!suzanne)
	{
		const std::string message = "Could not import model: " + std::string(importer.GetErrorString());
		throw std::runtime_error(message);
	}

	const aiMesh* mesh = suzanne->mMeshes[0];

	suzanne_geometry result;
	
	for (auto i = 0u; i < mesh->mNumVertices; ++i)
	{
		const auto& position = mesh->mVertices[i];
		result.vertices.emplace_back(position.x, position.y, position.z);

		const auto& uv = mesh->mTextureCoords[0][i];
		result.uvs.emplace_back(uv.x, uv.y);

		const auto& normal = mesh->mNormals[i];
		result.normals.emplace_back(normal.x, normal.y, normal.z);
	}

	for (auto i = 0u; i < mesh->mNumFaces; ++i)
	{
		const auto& face = mesh->mFaces[i];
		result.indices.push_back(face.mIndices[0]);
		result.indices.push_back(face.mIndices[1]);
		result.indices.push_back(face.mIndices[2]);
	}

	return result;
}

void main_loop::render_scene()
{
	const auto projection_matrix = m_camera->get_projection_matrix();
	const auto view_matrix = m_controls.get_view_matrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		// cube
		const auto model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 2));

		glBindVertexArray(m_cube_vertex_array);
		glUseProgram(m_cube_shaders.program.get_id());

		const auto mvp = projection_matrix * view_matrix * model_matrix;
		glUniformMatrix4fv(m_cube_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);

		const auto cube_position_attribute_location = glGetAttribLocation(m_cube_shaders.program.get_id(), "position");
		glEnableVertexAttribArray(cube_position_attribute_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_vertex_buffer);
		glVertexAttribPointer(cube_position_attribute_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		const auto cube_vertex_color_attribute_location = glGetAttribLocation(m_cube_shaders.program.get_id(), "vertex_color");
		glEnableVertexAttribArray(cube_vertex_color_attribute_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_cube_color_buffer);
		glVertexAttribPointer(cube_vertex_color_attribute_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		glDisableVertexAttribArray(cube_position_attribute_location);
		glDisableVertexAttribArray(cube_vertex_color_attribute_location);
	}

	{
		//suzanne
		auto model_matrix = glm::mat4(1.0f);
		model_matrix *= glm::scale(model_matrix, 2.f * glm::vec3(1.1f, 1.2f, 1.3f));
		const auto light_position = glm::vec3(4, 4, 4);

		glBindVertexArray(m_suzanne_vertex_array);
		glUseProgram(m_suzanne_shaders.program.get_id());

		const auto mvp = projection_matrix * view_matrix * model_matrix;
		glUniformMatrix4fv(m_suzanne_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(m_suzanne_model_matrix_uniform, 1, GL_FALSE, &model_matrix[0][0]);
		glUniformMatrix4fv(m_suzanne_view_matrix_uniform, 1, GL_FALSE, &view_matrix[0][0]);
		glUniform3f(m_suzanne_light_position_world_uniform, light_position.x, light_position.y, light_position.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_suzanne_texture);
		glUniform1i(m_suzanne_texture_shader_sampler, 0);

		const auto suzanne_vertex_position_attribute_location = glGetAttribLocation(m_suzanne_shaders.program.get_id(), "vertex_position_model");
		glEnableVertexAttribArray(suzanne_vertex_position_attribute_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_vertex_buffer);
		glVertexAttribPointer(suzanne_vertex_position_attribute_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		const auto suzanne_uv_attribute_location = glGetAttribLocation(m_suzanne_shaders.program.get_id(), "vertex_uv");
		glEnableVertexAttribArray(suzanne_uv_attribute_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_uv_buffer);
		glVertexAttribPointer(suzanne_uv_attribute_location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		const auto suzanne_normal_attribute_location = glGetAttribLocation(m_suzanne_shaders.program.get_id(), "vertex_normal_model");
		glEnableVertexAttribArray(suzanne_normal_attribute_location);
		glBindBuffer(GL_ARRAY_BUFFER, m_suzanne_normal_buffer);
		glVertexAttribPointer(suzanne_normal_attribute_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_suzanne_index_buffer);

		glDrawElements(GL_TRIANGLES, m_suzanne_indices, GL_UNSIGNED_SHORT, nullptr);

		glDisableVertexAttribArray(suzanne_normal_attribute_location);
		glDisableVertexAttribArray(suzanne_uv_attribute_location);
		glDisableVertexAttribArray(suzanne_vertex_position_attribute_location);
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