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

	const float diff = timestamp - m_first_timestamp;
	if (diff > 0.25f && get_rendered_frames() > 10)
	{
		m_fps = static_cast<uint16_t>(static_cast<float>(get_rendered_frames()) / (diff));
		m_first_timestamp = timestamp;
		m_n_frames = 0;
	}
}

uint16_t fps_counter::get_fps() const
{
	return m_fps;
}