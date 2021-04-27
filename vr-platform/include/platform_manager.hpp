#pragma once

#include "asset_manager.hpp"
#include "assimp_importer.hpp"

#include <memory>

namespace vr::platform
{
	class platform_manager
	{	
	public:
		virtual ~platform_manager() = default;

		virtual std::unique_ptr<asset_manager> get_asset_manager() = 0;
		virtual std::unique_ptr<assimp_importer> get_assimp_importer() = 0;
	};

}