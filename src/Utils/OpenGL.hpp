#pragma once

#include <QOpenGLFunctions>
#include <sstream>

class OpenGLErrorChecker : QOpenGLFunctions
{
public:
	OpenGLErrorChecker() { initializeOpenGLFunctions(); };
	void checkGLError()
	{
		GLenum err;
		bool throw_error = false;
		std::stringstream ss;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			ss << "ERROR: " << err << '\n';
			throw_error = true;
		}

		if (throw_error)
			throw std::runtime_error(ss.str().c_str());
	}
};
