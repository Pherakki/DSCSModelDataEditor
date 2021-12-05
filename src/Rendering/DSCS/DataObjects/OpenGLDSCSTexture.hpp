#pragma once
#include <string>
#include <QOpenGLFunctions>

namespace Rendering::DataObjects
{
	class OpenGLDSCSTexture : public QOpenGLFunctions
	{
	public:
		OpenGLDSCSTexture(GLuint buffer_id, std::string img_name);

		std::string img_name;

		GLuint getBufferID();
		void bind();
		void unbind();
	private:
		unsigned int buffer_id;
	};

}
