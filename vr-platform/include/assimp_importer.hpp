#pragma once

#include <assimp/Importer.hpp>
#include <assimp/IOSystem.hpp>

#include <memory>

namespace vr::platform
{
	class assimp_importer
	{
	public:
		assimp_importer(std::unique_ptr<Assimp::Importer> importer, std::unique_ptr<Assimp::IOSystem> iosystem);

		Assimp::Importer* get_importer();

	private:
		std::unique_ptr<Assimp::Importer> m_importer;
		std::unique_ptr<Assimp::IOSystem> m_iosystem;
	};
}