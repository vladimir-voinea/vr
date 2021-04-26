#include "pc_platform_manager.hpp"

#include "pc_asset_manager.hpp"

namespace vr::platform
{
	std::unique_ptr<asset_manager> pc_platform_manager::get_asset_manager()
	{
		return std::make_unique<pc_asset_manager>();
	}
}