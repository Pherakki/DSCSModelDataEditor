#pragma once
#include <string>

namespace Rendering::DataObjects
{
	class OpenGLDSCSTexture
	{
	public:
		OpenGLDSCSTexture(unsigned int buffer_id, std::string img_name);

		std::string img_name;

		unsigned int getBufferID();
		void bind();
		void unbind();
	private:
		unsigned int buffer_id;
	};

}
