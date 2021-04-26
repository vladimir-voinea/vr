#pragma once

#include "monkey.hpp"

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include <color_mateiral.hpp>
#include <vr-gl.hpp>
#include <vr.hpp>

#include <memory>

class main_loop
{
public:
	main_loop(int width, int height);
	~main_loop();

	void init();
	void render_scene(float delta_time);
	void resize(int width, int height);

	vr::camera& get_camera();

private:
	vr::mesh import_model(const std::string& name);
	void initialize_position();

	void print_state();

private:
	int m_width;
	int m_height;
	vr::perspective_camera::settings m_camera_settings;
	std::unique_ptr<vr::perspective_camera> m_camera;

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
	} m_monkey_data;
	const size_t n_monkeys = 5;
	std::vector<monkey> m_monkeys;

	int total_frames = 0;
};
