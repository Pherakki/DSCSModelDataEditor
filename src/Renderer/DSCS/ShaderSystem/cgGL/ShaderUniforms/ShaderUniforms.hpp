#pragma once

#include "../../../../Cg/cg.h"

namespace Rendering::DSCS::DataObjects::ShaderUniforms
{	
	// Base Class
	class AbstractcgGLShaderUniform
	{
	public:
		uint8_t id;
		CGparameter parameter;

		AbstractcgGLShaderUniform(uint8_t id, const CGparameter& parameter, float& static_data, float& anim_data)
		{
			this->id = id;
			this->parameter = parameter;
			this->static_data = &static_data;
			this->anim_data = &anim_data;
		}

		virtual void attach() noexcept = 0;
		void submit() 
		{ 
			this->anim_data[0] = this->static_data[0]; 
			this->anim_data[1] = this->static_data[1]; 
			this->anim_data[2] = this->static_data[2];
			this->anim_data[3] = this->static_data[3];
		}

		const std::string getName()
		{
			return std::string(cgGetParameterName(this->parameter));
		}
	protected:
		float * static_data;
		float * anim_data;
	};

	// VectorUniform
	class VectorUniform : public AbstractcgGLShaderUniform
	{
	private:
		uint8_t size;
	public:
		VectorUniform(uint8_t id, uint8_t size, const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(id, parameter, static_data, anim_data) { this->size = size; };
		void attach() noexcept { cgGLSetParameter4fv(this->parameter, this->anim_data); }
		uint8_t getSize() { return this->size; }
	};
	// Float4x4MatrixUniform
	class Float4x4MatrixUniform : public AbstractcgGLShaderUniform
	{
	public:
		Float4x4MatrixUniform(uint8_t id, const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(id, parameter, static_data, anim_data) {};
		void attach() noexcept 
		{ 
			cgGLSetMatrixParameterfc(this->parameter, this->anim_data); 
		}
	};

	// MatrixPaletteUniform
	class MatrixPaletteUniform : public AbstractcgGLShaderUniform
	{
	public:
		MatrixPaletteUniform(uint8_t id, const CGparameter& parameter, float& static_data, float& anim_data) : AbstractcgGLShaderUniform(id, parameter, static_data, anim_data) {};
		void attach() noexcept { cgGLSetParameterArray4f(this->parameter, 0, 56*3, this->anim_data); }
	};

}
