#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace vr
{
	using texel = glm::u8vec3;

	class texture
	{
	public:
		texture(const std::string& path);
		texture(std::vector<texel> texels, unsigned int width, unsigned int height);
		texture(std::vector<uint8_t> bytes);

		const std::string& get_path() const;

		bool texels_loaded_in_memory() const;
		const std::vector<texel>& get_texels() const;
		unsigned int get_width() const;
		unsigned int get_height() const;

		bool bytes_loaded_in_memory() const;
		const std::vector<uint8_t>& get_bytes() const;

	private:
		std::string m_path; 
		std::vector<texel> m_texels;
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		std::vector<uint8_t> m_bytes;
	};
}