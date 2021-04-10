#pragma once

#include <i_window_loop.hpp>

class main_loop : public vr::i_window_loop
{
public:
	main_loop();

	bool loop() override;
};
