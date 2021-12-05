#pragma once

#include "../../../../Cg/cg.h"

namespace Rendering::DSCS::DataObjects::ShaderUniforms
{	
	// Base Class
	class AbstractcgGLShaderUniform
	{
	public:
		AbstractcgGLShaderUniform(const CGparameter& parameter, float& static_data, float& anim_data)
		{
			this->parameter = parameter;
			this->static_data = &static_data;
			this->anim_data = &anim_data;
		}
		CGparameter parameter;

		virtual void attach() noexcept = 0;
		void submit() 
		{ 
			this->anim_data[0] = this->static_data[0]; 
			this->anim_data[1] = this->static_data[1]; 
			this->anim_data[2] = this->static_data[2];
			this->anim_data[3] = this->static_data[3];
		}
	protected:
		float * static_data;
		float * anim_data;
	};

	// VectorUniform
	class VectorUniform : public AbstractcgGLShaderUniform
	{
	public:
		VectorUniform(const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(parameter, static_data, anim_data) {};
		void attach() noexcept { cgGLSetParameter4fv(this->parameter, this->anim_data); }
	};
	// Float4x4MatrixUniform
	class Float4x4MatrixUniform : public AbstractcgGLShaderUniform
	{
	public:
		Float4x4MatrixUniform(const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(parameter, static_data, anim_data) {};
		void attach() noexcept 
		{ 
			cgGLSetMatrixParameterfc(this->parameter, this->anim_data); 
		}
	};

	// MatrixPaletteUniform
	class MatrixPaletteUniform : public AbstractcgGLShaderUniform
	{
	public:
		MatrixPaletteUniform(const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(parameter, static_data, anim_data) {};
		void attach() noexcept { cgGLSetParameterArray4f(this->parameter, 0, 56*3, this->anim_data); }
	};

}
