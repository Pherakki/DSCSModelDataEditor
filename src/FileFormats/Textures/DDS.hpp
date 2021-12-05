#pragma once

#include "../../../Cg/cg.h"
#include "../../../Cg/cgGL.h"
#include <stdio.h>
#include <QOpenGLFunctions>
#include "../../../src/Utils/OpenGL.hpp"

enum class TextureType
{
	Texture2D = 0,
	TextureCube = 1
};

// Adapted from https://gist.github.com/tilkinsc/d1a8a46853dea160dc86aa48618be6f9
class DDSLoader : public QOpenGLFunctions
{
public:
	DDSLoader();
	GLuint texture_loadDDS(const char* path, TextureType tex_type);

};
