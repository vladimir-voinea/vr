#include "shader_repository.hpp"

namespace vr::model
{
	std::string get_phong_vshader()
	{
		const char* vshader =
#include "phong.vshader"
			;

		return vshader;
	}
	
	std::string get_phong_fshader()
	{
		const char* fshader =
#include "phong.fshader"
			;

		return fshader;
	}
}