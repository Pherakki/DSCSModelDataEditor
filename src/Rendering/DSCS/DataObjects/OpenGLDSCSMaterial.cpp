#include "OpenGLDSCSMaterial.hpp"

namespace Rendering::DSCS::DataObjects
{

	// Constructor
	OpenGLDSCSMaterial::OpenGLDSCSMaterial(std::shared_ptr<ShaderObjects::cgGLShaderObject> shader)
	{
		for (auto& val : this->uniform_values)
			val = { 0., 0., 0., 0. };
		this->shader = shader;
		this->errorChecker = OpenGLErrorChecker();
	}

	// bind
	void OpenGLDSCSMaterial::bind()
	{
		this->shader->bind();
		for (auto& uniform : this->world_uniforms)
		{
			uniform->attach();
			this->errorChecker.checkGLError();
		}
		for (auto& uniform : this->material_uniforms)
		{
			uniform->attach();
			this->errorChecker.checkGLError();
		}
		for (auto& kv : this->texture_refs)
		{
			kv.second->bind();
			this->errorChecker.checkGLError();
		}
		for (int i = 0; i < this->opengl_settings.size(); ++i)
		{
			this->opengl_settings[i]->set();
			this->errorChecker.checkGLError();
		}
	}

	// unbind
	void OpenGLDSCSMaterial::unbind()
	{
		for (int i = 0; i < this->opengl_settings.size(); ++i)
		{
			this->opengl_settings[i]->unset();
		}
		this->shader->unbind();
	}


	// syncAnimationBuffer
	void OpenGLDSCSMaterial::syncAnimationBuffer()
	{
		for (auto& uniform : this->material_uniforms)
			uniform->submit();
	}

	// getTextureType
	uint16_t OpenGLDSCSMaterial::getTextureType(uint8_t id)
	{
		return this->texture_refs[id]->getTextureType();
	}

	// setTextureBuffer
	void OpenGLDSCSMaterial::setTextureBuffer(uint8_t id, uint16_t buffer_id)
	{
		this->texture_refs[id]->setBufferID(buffer_id);
	}

	void OpenGLDSCSMaterial::setUniformValue(uint8_t uniform_type_id, const std::array<float, 4>& uniform_data)
	{
		this->uniform_values[uniform_type_id] = uniform_data;
	}

	void OpenGLDSCSMaterial::setParamHandler(uint8_t id, const CGparameter& param, std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>>& holder, const std::array<float*, 0xA0>& uniform_dispatch_buffer)
	{
		CGtype param_type = cgGetParameterType(param);
		switch (param_type)
		{
		case CG_HALF:
		case CG_HALF1:
		case CG_HALF2:
		case CG_HALF3:
		case CG_HALF4:
		case CG_FLOAT:
		case CG_FLOAT1:
		case CG_FLOAT2:
		case CG_FLOAT3:
		case CG_FLOAT4:
			holder.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(param, this->uniform_values[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_FLOAT4x4:
			holder.emplace_back(std::make_unique<ShaderUniforms::Float4x4MatrixUniform>(param, this->uniform_values[id][0], *uniform_dispatch_buffer[id]));
			break;
		case CG_ARRAY:
			holder.emplace_back(std::make_unique<ShaderUniforms::MatrixPaletteUniform>(param, this->uniform_values[id][0], *uniform_dispatch_buffer[id]));
			break;
		default:
			std::string error_message = "Unhandled shader parameter type: ";
			error_message += cgGetTypeString(cgGetParameterType(param));
			throw std::exception(error_message.c_str());
			break;
		}
	}

	void OpenGLDSCSMaterial::setTextureHandler(uint8_t id, const CGparameter& param, std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>>& holder)
	{
		CGtype param_type = cgGetParameterType(param);
		switch (param_type)
		{
		case CG_SAMPLER2D:
			holder.emplace(id, std::make_unique<ShaderUniforms::Tex2DUniform>(param, 0));
			break;
		case CG_SAMPLERCUBE:
			holder.emplace(id, std::make_unique<ShaderUniforms::TexCubeUniform>(param, 0));
			break;
		default:
			std::string error_msg = "Unhandled shader parameter type: ";
			error_msg += cgGetTypeString(cgGetParameterType(param));
			throw std::exception(error_msg.c_str());
			break;
		}
	}

	void OpenGLDSCSMaterial::initShaderUniforms(const std::array<float*, 0xA0>& uniform_dispatch_buffer)
	{
		std::unordered_map<uint8_t, CGparameter> unlinked_uniforms;
		this->shader->getVertexParametersFromProgram(unlinked_uniforms);
		this->shader->getFragmentParametersFromProgram(unlinked_uniforms);

		// World Uniforms
		for (const uint8_t world_uniform_id : world_uniform_ids)
		{
			if (unlinked_uniforms.contains(world_uniform_id))
			{
				const CGparameter& param = unlinked_uniforms[world_uniform_id];
				this->setParamHandler(world_uniform_id, param, this->world_uniforms, uniform_dispatch_buffer);
				unlinked_uniforms.erase(world_uniform_id);
			}
		}
		// Textures
		for (const uint8_t texture_id : texture_ids)
		{
			if (unlinked_uniforms.contains(texture_id))
			{
				const CGparameter& param = unlinked_uniforms[texture_id];
				this->setTextureHandler(texture_id, param, this->texture_refs);
				unlinked_uniforms.erase(texture_id);
			}
		}
		// Everything else is a vector known to the material
		for (auto& kv : unlinked_uniforms)
		{
			uint8_t uniform_id = kv.first;
			const CGparameter& param = kv.second;
			this->material_uniforms.emplace_back(std::make_unique<ShaderUniforms::VectorUniform>(param, this->uniform_values[uniform_id][0], *uniform_dispatch_buffer[uniform_id]));
		}
	}

	void OpenGLDSCSMaterial::addOpenGLSetting(uint8_t setting_id, std::array<uint32_t, 4> payload)
	{
		switch (setting_id)
		{
		case 160: // glAlphaFunc
			float ref;
			memcpy(&ref, &payload[1], sizeof(ref));
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlAlphaFunc>(payload[0], ref));
			return;
		case 161: // GL_ALPHA_TEST
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0x0BC0, payload[0], false));
			return;
		case 162: // glBlendFunc
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlBlendFunc>(payload[0], payload[1]));
			return;
		case 163: // glBlendEquationSeparate
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlBlendEquationSeparate>(payload[0]));
			return;
		case 164: // GL_BLEND
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0x0BE2, payload[0], false));
			return;
		case 165: // glCullFace
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlCullFace>(payload[0]));
			return;
		case 166: // GL_CULL_FACE
			//this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0x0B44, payload[0], true));
			return;
		case 167: // glDepthFunc
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlDepthFunc>(payload[0]));
			return;
		case 168: // glDepthMask
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlDepthMask>(payload[0]));
			return;
		case 169: // GL_DEPTH_TEST
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0x0B71, payload[0], true));
			return;
			// case 170:
			// 	return;
			// case 171:
			// 	return;
		case 172: // glColorMask
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlColorMask>(payload[0], payload[1], payload[2], payload[3]));
			return;
		default:
			return;
			//throw std::exception("OpenGL setting not Recognised");
		}
	}
}