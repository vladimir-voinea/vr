#pragma once

#include <unordered_map>
#include <string>

namespace vr
{
	class cube_texture
	{
	public:
		static inline const std::string p_x = "px";
		static inline const std::string n_x = "nx";
		static inline const std::string p_y = "py";
		static inline const std::string n_y = "ny";
		static inline const std::string p_z = "pz";
		static inline const std::string n_z = "nz";
	public:
		cube_texture(const std::unordered_map<std::string, std::string>& paths);

		const std::unordered_map<std::string, std::string>& get_paths() const;
	
	private:
		std::unordered_map<std::string, std::string> m_paths;
	};
}