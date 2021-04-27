#include "assimp_importer.hpp"

namespace vr::platform
{
	assimp_importer::assimp_importer(std::unique_ptr<Assimp::Importer> importer, std::unique_ptr<Assimp::IOSystem> iosystem)
		: m_importer(std::move(importer))
		, m_iosystem(std::move(iosystem))
	{
		m_importer->SetIOHandler(m_iosystem.get());
	}

	Assimp::Importer* assimp_importer::get_importer()
	{
		return m_importer.get();
	}
}