#include "texture.hpp"

namespace vr
{
	texture::texture(const std::string& path)
		: m_path(path)
	{}

	texture::texture(const std::string& path, std::vector<texel> texels, unsigned int width, unsigned int height)
		: m_path(path)
		, m_texels(std::move(texels))
		, m_width(width)
		, m_height(height)
	{
	}

	const std::string& texture::get_path() const
	{
		return m_path;
	}

	const std::vector<texel>& texture::get_texels() const
	{
		return m_texels;
	}

	unsigned int texture::get_width() const
	{
		return m_width;
	}

	unsigned int texture::get_height() const
	{
		return m_height;
	}

	bool texture::is_loaded_in_memory() const
	{
		return !m_texels.empty();
	}
}