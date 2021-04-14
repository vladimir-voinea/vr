#pragma once

#include <memory>

namespace vr
{
	class renderer_object{};

	class has_renderer_object
	{
	public:
		renderer_object* get_renderer_object();

		template<typename type, typename... args>
		void make_renderer_object(args&&... arg)
		{
			m_renderer_object = std::make_unique<type>(std::forward<args>(arg)...);
		}

	private:
		std::unique_ptr<renderer_object> m_renderer_object;
	};
}