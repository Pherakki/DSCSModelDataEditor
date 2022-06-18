#pragma once

#include <sstream>


class OpenGLErrorChecker
{
public:
	void checkGLError();
};

void checkGLError();
void initGLAD();
