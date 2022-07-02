
#include "../../libs/glad/include/glad/glad.h"
#include "Renderer.hpp"

#include "../Utils/OpenGL.hpp"
#include "../FileFormats/DSCS/DSCStoOpenGL.hpp"

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

	ModelID_t Renderer::loadModel(const std::string& path)
	{
		//this->models.clear();
		auto id = models.size();
		models.emplace(id, std::make_shared<Model>(FileFormats::DSCS::DSCStoOpenGL(path, this->shader_backend, this->animation_buffer.uniform_dispatch_buffer, this->texture_library, this->shader_library)));
		auto& model = models.at(id);
		model->base_anim_sampler.setAnim(model->base_animation);
		model->base_anim_sampler.setSkel(model->skeleton);

		return id;
	}


	void Renderer::loadAnim(ModelID_t model_id, const std::string& anim_path)
	{
		auto& model = this->models[model_id];
		FileFormats::DSCS::loadAnimation<false>(*model, model->skeleton.getShaderChannelDataBlocks().size(), anim_path);
		model->anim_sampler_a.setAnim(model->animations[0]);
		model->anim_sampler_a.setSkel(model->skeleton);
	}

	void Renderer::render()
	{
		for (auto& kv : this->models)
		{
			auto& model = kv.second;
			auto& skeleton = model->skeleton;
			// Calculate the model's skeleton position for this frame
			//auto frame = model->base_anim_sampler.getFrame();
			//model->setSamplersFrame(35);
			model->sampleSkeletalAnimation();
			auto& bones = model->skeleton.getBoneDataBlocks();
			//model->setSamplersFrame(frame);
			// Now render each mesh
			for (int j = 0; j < model->meshes.size(); j++)
			{
				auto& mesh = model->meshes[j];
				mesh->checkGLError();

				// Load up the matrix palette with skeletal bone matrices
				for (uint16_t idx = 0; idx < mesh->used_bones.size(); ++idx)
				{
					for (uint8_t k = 0; k < 12; ++k)
					{
						this->animation_buffer.matrix_palette_buffer[12 * idx + k] = model->skeleton.transform_buffer[mesh->used_bones[idx]][k];
					}
				}

				// upload default shader uniform values to the animation buffer
				mesh->material->syncAnimationBuffer();
				// calculate shader uniform animations
				model->sampleShaderUniformAnimation(mesh->material, this->animation_buffer);
				// upload shader uniform animation values
				mesh->material->bind();
				mesh->checkGLError();

				this->shader_backend->checkBackendForCgError("Setting MVP...");
				mesh->bind();
				mesh->checkGLError();
				mesh->draw();
				mesh->checkGLError();
				mesh->unbind();
				mesh->checkGLError();
				mesh->material->unbind();

				mesh->checkGLError();
				this->shader_backend->checkBackendForCgError("Finishing unbind...");
			}
		}

		(*this->animation_buffer.Time)[0] = this->clock_time;
	}

	void Renderer::advTime(float adv)
	{
		this->delta_time = adv;
		this->clock_time += adv;
		for (auto& [id, model] : models)
			// Advance animation time
			model->tickSamplers(this->delta_time);
	}
}
