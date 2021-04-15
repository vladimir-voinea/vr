#pragma once

#include <stdint.h>

class fps_counter
{
public:
	fps_counter(float first_timestamp);

	void frame(float delta_time);
	uint16_t get_fps() const;

private:
	uint64_t get_rendered_frames() const;

private:
	uint64_t m_n_frames = 0;
	float m_first_timestamp = 0.0f;
	float m_current_timestamp = 0.0f;
	uint16_t m_fps;
};