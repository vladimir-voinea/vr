#pragma once

#include "monkey.hpp"

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include <color_mateiral.hpp>
#include <vr-gl.hpp>
#include <vr.hpp>

#include <memory>

struct viewport
{
	int x0;
	int y0;
	int width;
	int height;
};

class main_loop
{
public:
	main_loop(const viewport& viewport);
	~main_loop();

	void init();
	void frame(float delta_time);
	void resize(int width, int height);

	vr::camera& get_camera();

private:
	vr::mesh import_model(const std::string& name);
	void initialize_position();

	void print_state();

	void make_cameras();

private:
	viewport m_viewport;
	vr::perspective_camera::settings m_perspective_camera_settings;
	vr::orthographic_camera::settings m_orthographic_camera_settings;
	std::unique_ptr<vr::camera> m_perspective_camera;
	std::unique_ptr<vr::camera> m_orthographic_camera;

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
	} m_monkey_data;
	const size_t n_monkeys = 5;
	std::vector<monkey> m_monkeys;

	int total_frames = 0;
};
