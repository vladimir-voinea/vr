#pragma once

#include "shaders.hpp"
#include "user_controls.hpp"

#include <glfw_window.hpp>
#include <glfw_keyboard.hpp>
#include <glfw_mouse.hpp>
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>

#include <vr-gl.hpp>
#include <vr.hpp>

#include <memory>

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

	std::unique_ptr<vr::camera> m_camera;
	user_controls m_controls;
	
	float m_last_timestamp;
	float m_delta_time;

	struct monkey {
		vr::object3d* obj = nullptr;
		vr::geometry geometry;
		vr::shader_material* material = nullptr;
		std::vector<vr::gl::uniform> uniforms; 
		vr::texture* texture = nullptr;
		vr::mesh mesh;
	};
	monkey m_monkey;

	vr::scene m_scene;
	vr::gl::renderer m_renderer;
};
