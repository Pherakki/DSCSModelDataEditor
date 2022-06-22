
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

	void Renderer::refreshRenderSettings()
	{
		// Default settings
		glDisable(GL_ALPHA_TEST);
		glAlphaFunc(GL_ALWAYS, 0);

		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void Renderer::recalculateGlobalUniforms()
	{
		auto view_matrix = makeViewMatrix(this->camera.getPosition(), this->camera.getTarget(), this->camera.getUp());
		auto proj_matrix = perspectiveMatrix(this->camera.fov, this->aspect_ratio, this->camera.zNear, this->camera.zFar);
		auto viewproj = genericMatrixProduct<4, 4, 4>(view_matrix, proj_matrix);

		auto& light_dir = *this->animation_buffer.DirLamp01Dir;
		auto light_view_matrix = makeViewMatrix({ 0.f, 0.f, 0.f }, { light_dir[0], light_dir[1], light_dir[2] }, { 0.f, 1.f, 0.f });
		auto light_proj_matrix = perspectiveMatrix(50, this->aspect_ratio, 0.01, 1000);
		auto light_viewproj = genericMatrixProduct<4, 4, 4>(light_view_matrix, light_proj_matrix);

		this->animation_buffer.View->set(view_matrix);
		this->animation_buffer.ViewProj->set(viewproj);
		this->animation_buffer.LightViewProj->set(light_viewproj);
		this->animation_buffer.CameraPosition->set(this->camera.getPosition());
		this->animation_buffer.ViewInverse->set(invertViewMatrix(view_matrix, this->camera.getPosition()));
	}

}
