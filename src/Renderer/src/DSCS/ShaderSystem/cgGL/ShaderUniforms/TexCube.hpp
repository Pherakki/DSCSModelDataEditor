#pragma once

#include "BaseTexture.hpp"

namespace Rendering::DSCS::DataObjects::ShaderUniforms
{
	/// <summary>
	/// Tex2DUniform
	/// --------------
	/// Holds both a CGparameter and a pointer to texture data to be passed into the CGparameter on request.
	/// Call 'bind()' to load the held value into the CGparameter.
	/// </summary>
	class TexCubeUniform : public AbstractcgGLTextureReference
	{
	public:
		TexCubeUniform(CGparameter parameter, uint16_t buffer_id) : AbstractcgGLTextureReference(parameter, buffer_id, CG_SAMPLERCUBE)
		{};
		void bind() override
		{
			cgGLSetTextureParameter(this->parameter, this->buffer_id);
			cgGLEnableTextureParameter(this->parameter);
		};
	};

}
