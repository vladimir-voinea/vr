#include "platform_manager_factory.hpp"


#ifndef __ANDROID__
#include "pc_platform_manager.hpp"
#else
#include "android_platform_manager.hpp"
#endif

namespace vr::platform
{
	std::unique_ptr<platform_manager> get_platform_manager()
	{
#ifndef __ANDROID__
		return std::make_unique<pc_platform_manager>();
#else
		return std::make_unique<android_platform_manager>();
#endif
	}
}