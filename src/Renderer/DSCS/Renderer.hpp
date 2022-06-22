#pragma once

#include <memory>

#include "AnimationBuffer.hpp"
#include "DataObjects/OpenGLDSCSModel.hpp"
#include "RenderObjects/Camera.hpp"
#include "ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "UI/Types.hpp"

namespace Rendering::DSCS
{
	class Renderer
	{
		typedef std::shared_ptr<DataObjects::OpenGLDSCSModel> ModelPtr;
	public:
		Camera camera;
		ModelList_t models;
		AnimationBuffer animation_buffer;
		std::unique_ptr<ShaderBackends::cgGLShaderBackend> shader_backend;
		std::unordered_map<std::string, std::shared_ptr<ShaderObjects::cgGLShaderObject>> shader_library;

		Renderer();
	};
}
