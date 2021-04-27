#include "pc_platform_manager.hpp"

#include "pc_asset_manager.hpp"

#include <assimp/DefaultIOSystem.h>

namespace vr::platform
{
	std::unique_ptr<asset_manager> pc_platform_manager::get_asset_manager()
	{
		return std::make_unique<pc_asset_manager>();
	}

	std::unique_ptr<assimp_importer> pc_platform_manager::get_assimp_importer()
	{
		auto importer = std::make_unique<Assimp::Importer>();
		auto iosystem = std::make_unique<Assimp::DefaultIOSystem>();

		auto result = std::make_unique<assimp_importer>(std::move(importer), std::move(iosystem));

		return result;
	}
}