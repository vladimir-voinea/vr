#include "texture_loader.hpp"
#include <platform_manager_factory.hpp>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#include <regex>

namespace vr::gl
{
	GLuint load_stb(const std::string& path)
	{
		auto am = platform::get_platform_manager()->get_asset_manager();
		const auto contents = am->read_file(am->get_asset_by_name(path));

		int width = 0;
		int height = 0;
		int channels = 0;

		stbi_set_flip_vertically_on_load(true);
		auto stb_result = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, stb_result);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		
		stbi_image_free(stb_result);
		stbi_set_flip_vertically_on_load(false);

		return texture;
	}


	GLuint load_texture(const cube_texture* texture)
	{
		auto am = platform::get_platform_manager()->get_asset_manager();

		int width = 0;
		int height = 0;
		int channels = 0;

		std::vector<uint8_t> contents;
		stbi_set_flip_vertically_on_load(false);
		
		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::p_x)->second));
		auto px_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::n_x)->second));
		auto nx_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::p_y)->second));
		auto py_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::n_y)->second));
		auto ny_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::p_z)->second));
		auto pz_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		contents = am->read_file(am->get_asset_by_name(texture->get_paths().find(cube_texture::n_z)->second));
		auto nz_data = stbi_load_from_memory(contents.data(), contents.size(), &width, &height, &channels, STBI_rgb);

		GLuint texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, px_data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nx_data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, py_data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ny_data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pz_data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nz_data);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		stbi_image_free(px_data);
		stbi_image_free(nx_data);
		stbi_image_free(py_data);
		stbi_image_free(ny_data);
		stbi_image_free(pz_data);
		stbi_image_free(nz_data);

		return texture_id;
	}

	GLuint load_texture(const texture* texture)
	{
		std::regex e("(.*)(\\.DDS)", std::regex::ECMAScript | std::regex::icase);
		if (std::regex_match(texture->get_path(), e)) {

			return 0;
		}
		else
		{
			return load_stb(texture->get_path());
		}
	}
}