#pragma once

#include <sstream>


class OpenGLErrorChecker
{
public:
	OpenGLErrorChecker();
	void checkGLError();
};

void checkGLError();
