#include "android_platform_manager.hpp"

#include "android_asset_manager.hpp"

namespace vr::platform
{
	std::unique_ptr<asset_manager> android_platform_manager::get_asset_manager()
	{
		return std::make_unique<android_asset_manager>();
	}
}