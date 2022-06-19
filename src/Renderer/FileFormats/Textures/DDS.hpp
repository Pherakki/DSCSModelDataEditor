#pragma once

#include <stdio.h>
#include <QOpenGLFunctions_2_1>

enum class TextureType
{
	Texture2D,
	TextureCLUT,
	TextureLuminance,
	TextureCube
};

// Adapted from https://gist.github.com/tilkinsc/d1a8a46853dea160dc86aa48618be6f9
class DDSLoader : public QOpenGLFunctions_2_1
{
public:
	DDSLoader();
	GLuint texture_loadDDS(const char* path, TextureType tex_type);
};
