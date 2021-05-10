#pragma once

#include <vr-model.hpp>
#include <vr-gl.hpp>
#include <vr.hpp>

#include "parameters.hpp"

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
	void frame(float delta_time, const parameters& parameters);
	void resize(int width, int height);

	vr::camera& get_camera();

private:
	vr::mesh import_model(const std::string& name);
	void initialize_position();

	void print_state();

	void make_cameras();

	void transform_model(const parameters& parameters);

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

	std::unordered_map<std::string, std::pair<std::unique_ptr<vr::object3d>, vr::model::model_data>> m_loaded_models;

	std::string m_scene_model_path;
	vr::object3d* m_scene_model = nullptr;

	int total_frames = 0;
};
