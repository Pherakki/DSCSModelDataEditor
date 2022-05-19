#pragma once

#include "BaseTexture.hpp"
#include <QOpenGLFunctions>

namespace Rendering::DSCS::DataObjects::ShaderUniforms
{
	/// <summary>
	/// Tex2DUniform
	/// --------------
	/// Holds both a CGparameter and a pointer to texture data to be passed into the CGparameter on request.
	/// Call 'bind()' to load the held value into the CGparameter.
	/// </summary>
	class Tex2DUniform : public AbstractcgGLTextureReference, protected QOpenGLFunctions
	{
	public:
		Tex2DUniform(CGparameter parameter, uint16_t buffer_id) : AbstractcgGLTextureReference(parameter, buffer_id, CG_SAMPLER2D)
		{
			initializeOpenGLFunctions();
		};
		void bind() override
		{
			cgGLSetTextureParameter(this->parameter, this->buffer_id);
			cgGLEnableTextureParameter(this->parameter);
		};
	};
}
