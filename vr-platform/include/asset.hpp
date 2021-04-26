#pragma once

#include <string>

namespace vr::platform
{
	class asset
	{
	public:
		asset(const std::string& path);

		const std::string& get_path() const;

	private:
		std::string m_path;
	};
}