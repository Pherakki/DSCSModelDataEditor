#include "OpenGLDSCSTexture.hpp"

#include <glad/include/glad/glad.h>

namespace Rendering::DataObjects
{
	OpenGLDSCSTexture::OpenGLDSCSTexture(unsigned int buffer_id, std::string img_name)
	{
		this->buffer_id = buffer_id;
		this->img_name = img_name;
	}

	GLuint OpenGLDSCSTexture::getBufferID()
	{
		return this->buffer_id;
	}

	void OpenGLDSCSTexture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, this->buffer_id);
	}

	void OpenGLDSCSTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}