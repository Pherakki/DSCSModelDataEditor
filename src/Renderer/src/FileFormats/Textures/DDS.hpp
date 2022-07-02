#pragma once

#include <stdio.h>

enum class TextureType
{
	Texture2D,
	TextureCLUT,
	TextureLuminance,
	TextureCube
};

// Adapted from https://gist.github.com/tilkinsc/d1a8a46853dea160dc86aa48618be6f9
unsigned int loadDDS(const char* path, TextureType tex_type);
