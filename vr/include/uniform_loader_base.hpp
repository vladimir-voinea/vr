#include "uniform.hpp"

namespace vr
{
	class uniform_loader_base
	{
	public:
		virtual ~uniform_loader_base() = default;

		virtual void load_uniform(const vr::uniform& uniform) const = 0;
		virtual bool has_uniform(const std::string& name) const = 0;
	};
}