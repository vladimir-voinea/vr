#pragma once

#include "skybox.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace vr::gl
{
	struct viewport_t
	{
		int x0;
		int y0;
		int width;
		int height;

		int get_x0() const
		{
			return x0;
		}

		int get_x1() const
		{
			return get_x0() + width;
		}

		int get_y0() const
		{
			return y0;
		}

		int get_y1() const
		{
			return get_y0() + height;
		}
	};

	struct renderer_settings
	{
		viewport_t viewport;

		std::unique_ptr<glm::vec3> clear_color;
		std::unique_ptr<skybox_t> skybox;

		bool wireframe_mode = false;
		bool cull_faces = true;
	};
}