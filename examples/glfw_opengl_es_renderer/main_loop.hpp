#pragma once

#include "shaders.hpp"
#include "fps_counter.hpp"
#include "input_listener.hpp"
#include "monkey.hpp"

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include <color_mateiral.hpp>
#include <vr-gl.hpp>
#include <vr.hpp>

#include <memory>
#include <random>

class main_loop
{
public:
	main_loop(vr::glfw::window& window);
	~main_loop();

	void init();
	void run();

private:
	vr::mesh import_model(const std::string& name);
	void initialize_controls();
	void initialize_position();

	void process_input();
	void print_state();
	void render_scene();

private:
	vr::glfw::window& m_window;
	vr::glfw::keyboard m_kb;
	vr::glfw::mouse m_mouse;
	vr::perspective_camera::settings m_camera_settings;
	std::unique_ptr<vr::perspective_camera> m_camera;
	fps_counter m_fps_counter;
	input_listener m_input_listener;

	vr::gl::renderer_settings m_renderer_settings;
	std::unique_ptr<vr::gl::renderer> m_renderer;

	vr::scene m_scene;

	std::unique_ptr<vr::cube_texture> m_cube_texture;
	std::unique_ptr<vr::shader_material> m_skybox_material;

	float m_last_timestamp;
	float m_delta_time;

	struct {
		vr::geometry geometry;
		std::unique_ptr<vr::gl::opengl_shader> shader;
		std::unique_ptr<vr::texture> texture_uvmap;
		std::unique_ptr<vr::texture> texture_cobblestone;
		std::unique_ptr<vr::gl::color_material> material;
		std::default_random_engine m_random_engine;
	} m_monkey_data;
	const size_t n_monkeys = 5;
	std::vector<monkey> m_monkeys;

	int total_frames = 0;
};
