#pragma once

#include <memory>

#include "AnimationBuffer.hpp"
#include "DataObjects/OpenGLDSCSModel.hpp"
#include "RenderObjects/Camera.hpp"
#include "ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "RenderObjects/TextureLibrary.hpp"
#include "RenderObjects/Types.hpp"

namespace Rendering::DSCS
{
	class Renderer
	{
		typedef DataObjects::OpenGLDSCSModel Model;
		typedef std::shared_ptr<Model> ModelPtr;
	public:
		float aspect_ratio = 4.f/3;
		float clock_time = 0;
		float delta_time = 0;
		Camera camera;
		ModelList_t models;
		TextureLibrary texture_library;
		AnimationBuffer animation_buffer;
		std::unique_ptr<ShaderBackends::cgGLShaderBackend> shader_backend;
		std::unordered_map<std::string, std::shared_ptr<ShaderObjects::cgGLShaderObject>> shader_library;

		Renderer();
		void initRenderer(); // Call after creating an OpenGL context!
		void refreshRenderSettings();
		void recalculateGlobalUniforms();
		ModelPtr loadModel(const std::string& path);
		void loadAnim(const ModelPtr& model, const std::string& anim_path);
		void render();
		void advTime(float adv);
	};
}
