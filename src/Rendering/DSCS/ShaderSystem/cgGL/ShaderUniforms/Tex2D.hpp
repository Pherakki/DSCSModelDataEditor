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
			//glBindTexture(GL_TEXTURE_2D, buffer_id);
			cgGLSetTextureParameter(this->parameter, this->buffer_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			cgGLEnableTextureParameter(this->parameter);
		};
	};

	/// <summary>
	/// CLUTTex2DUniform
	/// --------------
	/// Holds both a CGparameter and a pointer to texture data to be passed into the CGparameter on request.
	/// Call 'bind()' to load the held value into the CGparameter.
	/// </summary>
	class CLUTTex2DUniform : public AbstractcgGLTextureReference, protected QOpenGLFunctions
	{
	public:
		CLUTTex2DUniform(CGparameter parameter, uint16_t buffer_id) : AbstractcgGLTextureReference(parameter, buffer_id, CG_SAMPLER2D)
		{
			initializeOpenGLFunctions();
		};
		void bind() override
		{
			//glBindTexture(GL_TEXTURE_2D, buffer_id);
			cgGLSetTextureParameter(this->parameter, this->buffer_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			cgGLEnableTextureParameter(this->parameter);
		};
	};
}
