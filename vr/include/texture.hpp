#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace vr
{
	using texel = glm::u8vec3;

	enum class wrap_mode
	{
		repeat,
		mirrored_repeat,
		clamp_to_edge,
		clamp_to_border
	};

	enum class operation
	{
		/** T = T1 + T2 */
		add,
		/** T = T1 * T2 */
		multiply,
		/** T = T1 - T2 */
		subtract,
		/** T = T1 / T2 */
		divide,
		/** T = (T1 + T2) - (T1 * T2) */
		smooth_add,
		/** T = T1 + (T2-0.5) */
		signed_add,
	};

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