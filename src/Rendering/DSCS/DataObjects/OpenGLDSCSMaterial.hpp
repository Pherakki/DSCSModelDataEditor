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
	public:
		OpenGLDSCSMaterial(std::shared_ptr<ShaderObjects::cgGLShaderObject> shader);
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
		void setTextureBuffer(uint8_t id, uint16_t buffer_id);
		void setTextureHandler(uint8_t id, const CGparameter& param, std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>>& holder);
		void setParamHandler(uint8_t id, const CGparameter& param, std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>>& holder, const std::array<float*, 0xA0>& uniform_dispatch_buffer);
		void setUniformValue(uint8_t uniform_type_id, const std::array<float, 4>& uniform_data);
		void initShaderUniforms(const std::array<float*, 0xA0>& uniform_dispatch_buffer);
		void addOpenGLSetting(uint8_t setting_id,  std::array<uint32_t, 4> inp);

	private:
		std::array<std::array<float, 4>, 0xA0> uniform_values;
		std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>> world_uniforms;
		std::vector<std::unique_ptr<ShaderUniforms::AbstractcgGLShaderUniform>> material_uniforms;
		std::map<uint8_t, std::unique_ptr<ShaderUniforms::AbstractcgGLTextureReference>> texture_refs;
		std::vector<std::shared_ptr<OpenGLSettings::OpenGLSetting>> opengl_settings;
};


}
