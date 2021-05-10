#include "preamble.hpp"
#include <glfw_util.hpp>

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

preamble::preamble(vr::glfw::window& window)
	: m_window(window)
	, m_kb(window)
	, m_mouse(window)
	, m_fps_counter(vr::glfw::get_time())
{
	initialize();
}

void preamble::initialize()
{
	initialize_glew();

	m_window.set_framebuffer_resize_callback([this](int width, int height)
		{
			m_main_loop->resize(width, height);
		});

	const auto viewport_size = m_window.get_viewport_size();
	m_main_loop = std::make_unique<main_loop>(viewport{ 0, 0, viewport_size.width, viewport_size.height });
	m_input_listener = std::make_unique<input_listener>(m_window, m_main_loop->get_camera(), m_fps_counter);
	initialize_controls();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window.get_handle(), false);
	ImGui_ImplOpenGL3_Init("#version 300 es");
}

void preamble::initialize_glew()
{
	glewExperimental = GL_TRUE;
	const auto glew_initialization = glewInit();
	if (glew_initialization != GLEW_OK)
	{
		throw std::runtime_error("Could not initialize glew");
	}
}

void preamble::initialize_controls()
{
	m_kb.set_listener(m_input_listener.get());
	m_mouse.set_listener(m_input_listener.get());
	m_mouse.set_mode(vr::glfw::mouse_mode::disabled);
	m_mouse.set_raw_motion(true);
	const auto viewport_size = m_window.get_viewport_size();
	const auto mouse_position = vr::glfw::mouse_position{ static_cast<double>(viewport_size.width / 2), static_cast<double>(viewport_size.height / 2) };
	m_mouse.set_position(mouse_position);

	m_kb.set_sticky_keys(true);
	m_mouse.set_sticky_buttons(true);
}

void preamble::run_loop()
{
	while (!m_window.close_requested())
	{
		if (m_window.has_focus())
		{
			const auto current_time = static_cast<float>(vr::glfw::get_time());
			const auto delta_time = current_time - m_last_timestamp;
			m_last_timestamp = current_time;
			m_fps_counter.frame(current_time);

			process_input();
			m_main_loop->frame(delta_time);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			float f;
			ImGui::Text("Hello, world %d", 123);
			if (ImGui::Button("Save")) {
				spdlog::info("Save clicked!");
			}
			char buf[256] = "asdf";
			ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			m_window.swap_buffers();
		}

		vr::glfw::poll_events();
	}
}

void preamble::process_input()
{
	if (m_kb.get_key_state(vr::glfw::key::escape) == vr::glfw::key_action::press)
	{
		m_window.request_close();
	}

}