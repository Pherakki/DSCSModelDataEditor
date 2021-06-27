#pragma once

#include <memory>
#include <vector>
#include<Rendering/ShaderBackends/cgGL/cgGLShaderObject.hpp>

namespace Rendering::DataObjects
{
	class AbstractShaderUniform
	{
		// Should probably know how to pass itself into a shader, whether that's Cg or GLSL
	};

	class OpenGLSetting
	{
		// Should hold a lookup key and a payload
	};
	
	class OpenGLDSCSMaterial
	{
	public:
		// Need to map uniform IDs and opengl IDs to parameter names and functions
		std::string name;
		std::shared_ptr<ShaderObjects::cgGLShaderObject> shader;
		std::vector<AbstractShaderUniform> uniforms;
		std::vector<OpenGLSetting> opengl_settings;

		void bind();
		void unbind();
	};

	void OpenGLDSCSMaterial::bind()
	{
		this->shader->bind();
	}

	void OpenGLDSCSMaterial::unbind()
	{
		this->shader->unbind();
	}



}
