#include <uniform_loader_base.hpp>

#include "shader_program.hpp"

namespace vr::gl
{
	class uniform_loader : public uniform_loader_base
	{
	public:
		uniform_loader(const shader_program& program);
		~uniform_loader() override;

		void load_uniform(const vr::uniform& uniform) const override;
		bool has_uniform(const std::string& name) const override;

	private:
		const shader_program& m_program;
	};
}