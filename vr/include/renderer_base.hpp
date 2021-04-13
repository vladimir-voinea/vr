#include "scene.hpp"
#include "camera.hpp"

namespace vr
{
	class renderer_base
	{
	public:
		virtual void render(const vr::scene& scene, const vr::camera& camera) = 0;
	};
}