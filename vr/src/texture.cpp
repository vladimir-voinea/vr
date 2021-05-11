#include "texture.hpp"

namespace vr
{
	texture::texture(const std::string& path)
		: m_path(path)
	{}

	texture::texture(std::vector<texel> texels, unsigned int width, unsigned int height)
		: m_texels(std::move(texels))
		, m_width(width)
		, m_height(height)
	{
	}

	texture::texture(std::vector<uint8_t> bytes)
		: m_bytes(std::move(bytes))
	{}

	const std::string& texture::get_path() const
	{
		return m_path;
	}

	bool texture::texels_loaded_in_memory() const
	{
		return !m_texels.empty();
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

	bool texture::bytes_loaded_in_memory() const
	{
		return !m_bytes.empty();
	}

	const std::vector<uint8_t>& texture::get_bytes() const
	{
		return m_bytes;
	}
}