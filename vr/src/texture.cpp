#include "texture.hpp"

namespace vr
{
	texture::texture(const std::string& path)
		: m_path(path)
	{}

	const std::string& texture::get_path() const
	{
		return m_path;
	}
}