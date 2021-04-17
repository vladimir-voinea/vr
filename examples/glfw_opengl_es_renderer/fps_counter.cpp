#include "fps_counter.hpp"

fps_counter::fps_counter(float first_timestamp)
	: m_first_timestamp(first_timestamp)
{}

uint64_t fps_counter::get_rendered_frames() const
{
	return m_n_frames;
}

void fps_counter::frame(float timestamp)
{
	++m_n_frames;

	m_delta_time = timestamp - m_first_timestamp;
	if (m_delta_time > 0.25f)
	{
		m_fps = static_cast<uint16_t>(static_cast<float>(get_rendered_frames()) / (m_delta_time));
		m_first_timestamp = timestamp;
		m_n_frames = 0;
	}
}

uint16_t fps_counter::get_fps() const
{
	return m_fps;
}

float fps_counter::get_time_since_last_frame() const
{
	return m_delta_time;
}