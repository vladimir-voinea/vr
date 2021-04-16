#include "cube_texture.hpp"

#include <spdlog/spdlog.h>

namespace vr
{
	cube_texture::cube_texture(const std::unordered_map<std::string, std::string>& paths)
		: m_paths(paths)
	{
		if (m_paths.size() != 6)
		{
			spdlog::error("Cube textures need 6 texture files. Have {0}", m_paths.size());
		}
	}

	const std::unordered_map<std::string, std::string>& cube_texture::get_paths() const
	{
		return m_paths;
	}
}