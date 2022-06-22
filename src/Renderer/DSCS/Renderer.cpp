
#include <glad/include/glad/glad.h>
#include "Renderer.hpp"

#include "Utils/OpenGL.hpp"

namespace Rendering::DSCS
{
	Renderer::Renderer()
	{
		this->shader_backend = std::make_unique<Rendering::ShaderBackends::cgGLShaderBackend>();

		this->animation_buffer.DirLamp01Dir->set({ 1.0f, 0.f, 1.0f });
		this->animation_buffer.DirLamp01Color->set({ 0.5f, 0.5f, 0.5f, 0.5f });
		this->animation_buffer.AmbientColor->set({ 0.5f, 0.5f, 0.5f });
		this->animation_buffer.FogColor->set({ 0.5f, 0.1f, 0.0f });
		this->animation_buffer.FogParams->set({ 0.01f, 1.0f, 0.5f });
		this->animation_buffer.GroundColor->set({ 0.f, 0.5f, 0.0f });
		this->animation_buffer.SkyDir->set({ 0.0f, 1.0f, 0.0f });
		// BackBufferSampler
		// ShadowSampler

		// Set up camera
		this->camera.setPosition({ 0.0f, 0.0f, 3.f });
	}

	// Call after creating an OpenGL context!
	void Renderer::initRenderer()
	{
		initGLAD();
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		this->refreshRenderSettings();
	}

}
