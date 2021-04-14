#pragma once

#include <string>

namespace vr
{
	class texture
	{
	public:
		texture(const std::string& path);

		const std::string& get_path() const;

	private:
		std::string m_path;
	};
}