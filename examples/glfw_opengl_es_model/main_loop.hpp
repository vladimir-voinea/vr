#pragma once

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>

#include <assimp/Importer.hpp>
#include "geometry_data_base.hpp"

#include <shader.hpp>
#include <shader_program.hpp>
#include "shaders.hpp"

#include <camera.hpp>
#include "user_controls.hpp"

#include <glm/glm.hpp>

#include <memory>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();
	void run();

private:
	using suzanne_geometry = geometry_data_base<glm::vec3, glm::vec2, glm::vec3, uint16_t>;
	suzanne_geometry import_model();
	void initialize_controls();
	void initialize_position();

	void process_input();
	void print_state();
	void render_scene();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;

	std::unique_ptr<vr::camera> m_camera;
	user_controls m_controls;
	
	double m_last_timestamp;

	shaders m_cube_shaders;
	GLuint m_cube_mvp_uniform;
	GLuint m_cube_vertex_array;
	GLuint m_cube_vertex_buffer;
	GLuint m_cube_color_buffer;

	shaders m_suzanne_shaders;
	GLuint m_suzanne_mvp_uniform;
	GLuint m_suzanne_view_matrix_uniform;
	GLuint m_suzanne_model_matrix_uniform;
	GLuint m_suzanne_light_position_world_uniform;
	GLuint m_suzanne_vertex_array;
	GLuint m_suzanne_vertex_buffer;
	GLuint m_suzanne_uv_buffer;
	GLuint m_suzanne_normal_buffer;
	GLuint m_suzanne_index_buffer;
	uint64_t m_suzanne_indices;

	GLuint m_suzanne_texture;
	GLuint m_suzanne_texture_shader_sampler;
};
