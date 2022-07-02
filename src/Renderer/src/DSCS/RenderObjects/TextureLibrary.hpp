#pragma once

#include <unordered_map>

struct TextureLibrary
{
	// Unsigned int should be GLuint
	typedef std::unordered_map<std::string, unsigned int> TextureSubLibrary_t;

	TextureSubLibrary_t textures_2d;
	TextureSubLibrary_t textures_clut;
	TextureSubLibrary_t textures_luminance;
	TextureSubLibrary_t textures_cube;
};
