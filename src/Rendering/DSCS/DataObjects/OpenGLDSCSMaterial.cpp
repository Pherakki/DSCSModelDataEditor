#include "OpenGLDSCSMaterial.hpp"

namespace Rendering::DSCS::DataObjects
{
	typedef std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>> ShaderUniformVec_t;
	typedef std::vector<std::unique_ptr<ShaderUniforms::VectorUniform>> VectorUniformVec_t;
	typedef std::shared_ptr<ShaderObjects::cgGLShaderObject> ShaderPtr;

	// Constructor
	OpenGLDSCSMaterial::OpenGLDSCSMaterial(const ShaderPtr& shader, const std::array<float*, 0xA0>& uniform_dispatch_buffer)
	{
		for (auto& val : this->local_uniform_buffer)
			val = { 0., 0., 0., 0. };
		this->shader = shader;
		this->errorChecker = OpenGLErrorChecker();
		this->initShaderUniforms(uniform_dispatch_buffer);
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

	// setTextureName
	void OpenGLDSCSMaterial::setTextureName(uint8_t id, const std::string& img_name)
	{
		this->texture_refs[id]->setName(img_name);
	}

	// setTextureBuffer
	void OpenGLDSCSMaterial::setTextureBuffer(uint8_t id, uint16_t buffer_id)
	{
		this->texture_refs[id]->setBufferID(buffer_id);
	}

	void OpenGLDSCSMaterial::setUniformValue(uint8_t uniform_type_id, const std::array<float, 4>& uniform_data)
	{
		this->local_uniform_buffer[uniform_type_id] = uniform_data;
	}

	void OpenGLDSCSMaterial::setTextureHandler(uint8_t id, const CGparameter& param, std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>>& holder)
	{
		CGtype param_type = cgGetParameterType(param);
		switch (param_type)
		{
		case CG_SAMPLER2D:
			//if (id == 0x48)
			//	holder.emplace(id, std::make_unique<ShaderUniforms::CLUTTex2DUniform>(param, 0));
			//else
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
			this->setParamHandler(uniform_id, param, this->material_uniforms, uniform_dispatch_buffer);
		}
	}

	void OpenGLDSCSMaterial::addOpenGLSetting(uint8_t setting_id, std::array<uint32_t, 4> payload)
	{
		switch (setting_id)
		{
		case 0xA0: // glAlphaFunc
			float ref;
			memcpy(&ref, &payload[1], sizeof(ref));
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlAlphaFunc>(0xA0, payload[0], ref));
			return;
		case 0xA1: // GL_ALPHA_TEST
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0xA1, 0x0BC0, payload[0], false));
			return;
		case 0xA2: // glBlendFunc
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlBlendFunc>(0xA2, payload[0], payload[1]));
			return;
		case 0xA3: // glBlendEquationSeparate
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlBlendEquationSeparate>(0xA3, payload[0]));
			return;
		case 0xA4: // GL_BLEND
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0xA4, 0x0BE2, payload[0], false));
			return;
		case 0xA5: // glCullFace
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlCullFace>(0xA5, payload[0]));
			return;
		case 0xA6: // GL_CULL_FACE
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0xA6, 0x0B44, payload[0], true));
			return;
		case 0xA7: // glDepthFunc
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlDepthFunc>(0xA7, payload[0]));
			return;
		case 0xA8: // glDepthMask
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlDepthMask>(0xA8, payload[0]));
			return;
		case 0xA9: // GL_DEPTH_TEST
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlEnable>(0xA9, 0x0B71, payload[0], true));
			return;
			// case 0xAA:
			// 	return;
			// case 0xAB:
			// 	return;
		case 0xAC: // glColorMask
			this->opengl_settings.emplace_back(std::make_shared<OpenGLSettings::OpenGLSettingGlColorMask>(0xAC, payload[0], payload[1], payload[2], payload[3]));
			return;
		default:
			return;
			//throw std::exception("OpenGL setting not Recognised");
		}
	}
}