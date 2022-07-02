#pragma once

#include "../../../../../libs/Cg/cgGL.h"

namespace Rendering::DSCS::DataObjects::ShaderUniforms
{
	// Base Class
	class AbstractcgGLTextureReference
	{
	public:
		AbstractcgGLTextureReference(const CGparameter& parameter, uint16_t buffer_id, uint16_t input_texture_type) : texture_type{ input_texture_type }
		{
			this->parameter = parameter; this->buffer_id = buffer_id;
		};
		std::string img_name;
		CGparameter parameter;
		uint16_t buffer_id;
		const uint16_t texture_type;

		virtual void bind() = 0;
		uint16_t getTextureType() { return this->texture_type; };
		void setBufferID(uint16_t buffer_id) { this->buffer_id = buffer_id; };
		void setName(const std::string& img_name) { this->img_name = img_name; };
	};
}
