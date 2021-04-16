#pragma once

#include <string>
#include <optional>

namespace vr::glfw
{
	struct version
	{
		uint16_t major;
		uint16_t minor;
	};

	enum class opengl_context_api
	{
		opengl_es,
		opengl
	};

	enum class opengl_profile
	{
		core,
		any
	};

	struct opengl_context
	{
		opengl_context_api api;
		version context_version;
		opengl_profile profile;
		bool foward_compatible;
	};

	struct window_settings
	{
		int width = 800;
		int height = 600;
		bool full_screen = false;
		std::string name = "Unnamed window";
		std::optional<opengl_context> opengl_context = {};
		bool resizable = false;
		std::optional<uint8_t> msaa_samples;
	};
}