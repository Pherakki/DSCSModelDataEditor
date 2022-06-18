#include "../glad/include/glad/glad.h"

#include "OpenGL.hpp"

OpenGLErrorChecker::OpenGLErrorChecker()
{ 
	gladLoadGL(); // This should only be called once in main, but since the code currently mixes GLAD and Qt5, need to do it here
}

void OpenGLErrorChecker::checkGLError()
{
	::checkGLError();
}

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
