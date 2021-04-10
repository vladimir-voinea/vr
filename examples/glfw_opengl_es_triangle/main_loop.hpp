#pragma once

#include <i_window_loop.hpp>
#include <glfw_window.hpp>

class main_loop : public vr::i_window_loop
{
public:
	main_loop(vr::glfw_window& window);

	bool loop() override;

private:
	vr::glfw_window& m_window;
};
