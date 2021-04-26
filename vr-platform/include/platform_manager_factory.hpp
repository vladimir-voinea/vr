#pragma once

#include "platform_manager.hpp"

#include <memory>

namespace vr::platform
{
	std::unique_ptr<platform_manager> get_platform_manager();
}
