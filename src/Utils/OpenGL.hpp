#pragma once

#include <QOpenGLFunctions>
#include <sstream>
#include <debugapi.h>

class OpenGLErrorChecker : QOpenGLFunctions
{
public:
	OpenGLErrorChecker() { initializeOpenGLFunctions(); };
	void checkGLError()
	{
		GLenum err;
		bool throw_error = false;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::stringstream ss;
			ss << "ERROR: " << err << '\n' << std::endl;
			OutputDebugStringA(ss.str().c_str());
			throw_error = true;
		}

		if (throw_error)
			throw std::exception();
	}
};
