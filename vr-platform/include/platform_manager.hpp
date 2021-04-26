#pragma once

#include "asset_manager.hpp"

#include <memory>

namespace vr::platform
{
	class platform_manager
	{	
	public:
		virtual std::unique_ptr<asset_manager> get_asset_manager() = 0;
	};

}