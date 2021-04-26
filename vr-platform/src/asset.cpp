#include <asset.hpp>

namespace vr::platform
{
	asset::asset(const std::string& path)
		: m_path(path)
	{}

	const std::string& asset::get_path() const
	{
		return m_path;
	}
}