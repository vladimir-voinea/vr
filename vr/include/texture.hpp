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
		texture(const std::string& path, std::vector<texel> texels, unsigned int width, unsigned int height);

		const std::string& get_path() const;
		const std::vector<texel>& get_texels() const;

		unsigned int get_width() const;
		unsigned int get_height() const;

		bool is_loaded_in_memory() const;

	private:
		std::string m_path; 
		std::vector<texel> m_texels;
		unsigned int m_width = 0;
		unsigned int m_height = 0;
	};
}