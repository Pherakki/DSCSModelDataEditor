#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include "../../../../Cg/cg.h"

#include "OpenGLDSCSTexture.hpp"
#include "../Renderer.hpp"
#include "../ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "../ShaderSystem/cgGL/ShaderVariables.hpp"
#include "../ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp"
#include "../ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
#include "../ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp"
#include "../ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
#include "../ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
#include "../../../Utils/OpenGL.hpp"

namespace Rendering::DSCS::DataObjects
{
	struct TextureReferenceData
	{
		uint16_t texture_id;
		uint16_t buffer_id;
		uint16_t uniform_type;
		uint16_t padding_1;
		uint16_t padding_2;
		uint16_t padding_3;
		uint16_t salt_1;
		uint16_t salt_2;
	};

	class OpenGLDSCSMaterial
	{
	private:
		typedef std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>> ShaderUniformVec_t;
		typedef std::vector<std::unique_ptr<ShaderUniforms::VectorUniform>> VectorUniformVec_t;
		typedef std::shared_ptr<ShaderObjects::cgGLShaderObject> ShaderPtr;
		std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>> world_uniforms;
	public:
		OpenGLDSCSMaterial(const ShaderPtr& shader, const std::array<float*, 0xA0>& uniform_dispatch_buffer);
		// Need to map uniform IDs and opengl IDs to parameter names and functions
		// Lots of these should be private and have getters
		std::string name;
		uint32_t name_hash;
		std::shared_ptr<ShaderObjects::cgGLShaderObject> shader;
		OpenGLErrorChecker errorChecker;

		void bind();
		void unbind();
		void syncAnimationBuffer();
		uint16_t getTextureType(uint8_t id);
		void setTextureName(uint8_t id, const std::string& img_name);
		void setTextureBuffer(uint8_t id, uint16_t buffer_id);
		void setTextureHandler(uint8_t id, const CGparameter& param, std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>>& holder);
		template<typename T>
		void setParamHandler(uint8_t id, const CGparameter& param, T& holder, const std::array<float*, 0xA0>& uniform_dispatch_buffer);
		void setUniformValue(uint8_t uniform_type_id, const std::array<float, 4>& uniform_data);
		void initShaderUniforms(const std::array<float*, 0xA0>& uniform_dispatch_buffer);
		void addOpenGLSetting(uint8_t setting_id,  std::array<uint32_t, 4> inp);
		std::array<std::array<float, 4>, 0xA0> local_uniform_buffer;

		std::vector<std::unique_ptr<ShaderUniforms::VectorUniform>> material_uniforms;
		std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>> texture_refs;
		std::vector<std::shared_ptr<OpenGLSettings::OpenGLSetting>> opengl_settings;
	};

	template<typename T>
	void OpenGLDSCSMaterial::setParamHandler(uint8_t id, const CGparameter& param, T& holder, const std::array<float*, 0xA0>& uniform_dispatch_buffer)
	{
		CGtype param_type = cgGetParameterType(param);
		switch (param_type)
		{
		case CG_HALF:
		case CG_FLOAT:
		case CG_HALF1:
		case CG_FLOAT1:
			holder.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(id, 1, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_HALF2:
		case CG_FLOAT2:
			holder.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(id, 2, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_HALF3:
		case CG_FLOAT3:
			holder.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(id, 3, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_HALF4:
		case CG_FLOAT4:
			holder.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(id, 4, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_FLOAT4x4:
			if constexpr (std::is_same<T, ShaderUniformVec_t>::value)
			{
				holder.emplace_back(std::make_unique<ShaderUniforms::Float4x4MatrixUniform>(id, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
				break;
			}
		case CG_ARRAY:
			if constexpr (std::is_same<T, ShaderUniformVec_t>::value)
			{
				holder.emplace_back(std::make_unique<ShaderUniforms::MatrixPaletteUniform>(id, param, this->local_uniform_buffer[id][0], *uniform_dispatch_buffer[id]));
				break;
			}
		default:
			std::string error_message = "Unhandled shader parameter type: ";
			error_message += cgGetTypeString(cgGetParameterType(param));
			throw std::exception(error_message.c_str());
			break;
		}
	}
}
