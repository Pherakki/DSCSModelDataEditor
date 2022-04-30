#include "RenderWidget.hpp"

typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
typedef std::shared_ptr<Model> ModelPtr;

namespace CustomWidgets
{
	// Constructor
	RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
	{
		this->shader_backend = std::make_unique<Rendering::ShaderBackends::cgGLShaderBackend>();
		// Set up the render loop
		connect(&this->clock, &QTimer::timeout, this, &RenderWidget::update);
		this->clock.start(this->targetFrameUpdateTime);
		this->setFocusPolicy(Qt::StrongFocus);

		// Set up camera
		this->camera.setPosition({ 0.0f, 0.0f, 3.f });

		// DirLampDir01
		this->animation_buffer.shader_uniform_buffer[0xB0] = 1.f;
		this->animation_buffer.shader_uniform_buffer[0xB1] = 0.f;
		this->animation_buffer.shader_uniform_buffer[0xB2] = 1.f;
		this->animation_buffer.shader_uniform_buffer[0xB3] = 0.f;

		// DirLampColor01
		this->animation_buffer.shader_uniform_buffer[0xC0] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0xC1] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0xC2] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0xC3] = 0.5f;

		// AmbientColor
		this->animation_buffer.shader_uniform_buffer[0x2E0] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0x2E1] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0x2E2] = 0.5f;
		this->animation_buffer.shader_uniform_buffer[0x2E3] = 0.5f;
	}

	void RenderWidget::refreshRenderSettings()
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

	void RenderWidget::initializeGL()
	{
		initializeOpenGLFunctions();
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		this->refreshRenderSettings();

		std::cout << "Render Widget Init" << std::endl;
		emit this->glInitialised();
	}

	ModelPtr RenderWidget::loadModel(const std::string& path)
	{
		this->models.clear();
		models.emplace(0, std::make_shared<Model>(FileFormats::DSCS::DSCStoOpenGL(path, this->shader_backend, this->animation_buffer.uniform_dispatch_buffer, this->texture_library, this->shader_library)));
		auto& model = models.at(0);
		model->base_anim_sampler.setAnim(model->base_animation);
		model->base_anim_sampler.setSkel(model->skeleton);

		return model;
	}


	void RenderWidget::loadAnim(const std::string& anim_path)
	{
		auto& model = this->models.at(0);
		FileFormats::DSCS::loadAnimation<false>(*model, model->skeleton.getShaderChannelDataBlocks().size(), anim_path);
		model->anim_sampler.setAnim(model->animations[0]);
		model->anim_sampler.setSkel(model->skeleton);
	}

	RenderWidget::~RenderWidget()
	{

	}


	void RenderWidget::update()
	{
		this->registerMousePosition();
/*		if (this->input_handler.attemptedSelect())
		{
			auto& mouse_pos = this->input_handler.getMousePos();
			float x = (2.0f * mouse_pos.x()) / this->width() - 1.0f;
			float y = 1.0f - (2.0f * mouse_pos.y()) / this->height();

			auto homogeneous_clip_ray = std::array<float, 4>{x, y, -1.f, 1.f};
			std::array<float, 16> view_inverse;

			auto inv_proj_matrix = inversePerspectiveMatrix(this->camera.fov, this->aspect_ratio, this->camera.zNear, this->camera.zFar);
			auto eye_ray = genericMatrixProduct<4, 4, 1>(inv_proj_matrix, homogeneous_clip_ray);

			memcpy(view_inverse.data(), &(*this->animation_buffer.ViewInverse)[0], sizeof(float) * 16);
			auto world_ray = genericMatrixProduct<4, 4, 1>(view_inverse, eye_ray);
		}
		else */if (this->input_handler.shouldTranslateCamera())
		{
			auto& mdelta = this->input_handler.getMouseDelta();
			this->camera.translate(mdelta.x(), mdelta.y());
		}
		else if (this->input_handler.shouldRotateCamera())
		{
			auto& mdelta = this->input_handler.getMouseDelta();
			this->camera.incAltAzi(mdelta.x(), mdelta.y());
		}

		auto view_matrix = makeViewMatrix(this->camera.getPosition(), this->camera.getTarget(), this->camera.getUp());
		auto proj_matrix = perspectiveMatrix(this->camera.fov, this->aspect_ratio, this->camera.zNear, this->camera.zFar);
		auto viewproj = genericMatrixProduct<4, 4, 4>(view_matrix, proj_matrix);

		this->animation_buffer.View->set(view_matrix);
		this->animation_buffer.ViewProj->set(viewproj);
		this->animation_buffer.CameraPosition->set(this->camera.getPosition());
		this->animation_buffer.ViewInverse->set(invertViewMatrix(view_matrix, this->camera.getPosition()));

		this->increment_test += 0.001*targetFrameUpdateTime; // convert to seconds
		//std::cout << this->increment_test << std::endl;
		this->repaint();
	}

	void RenderWidget::paintGL()
	{
		this->refreshRenderSettings();
		for (auto& kv: this->models)
		{
			auto& model = kv.second;
			auto& skeleton = model->skeleton;
			model->base_anim_sampler.sampleCurrentFrame(skeleton.quat_buffer, skeleton.loc_buffer, skeleton.scale_buffer, this->animation_buffer.shader_uniform_buffer);
			model->anim_sampler.sampleCurrentFrame(skeleton.quat_buffer, skeleton.loc_buffer, skeleton.scale_buffer);
			model->skeleton.computeTransformBuffer();
			auto& bones = model->skeleton.getBoneDataBlocks();
			for (int j = 0; j < model->meshes.size(); j++)
			{
				auto& mesh = model->meshes[j];
				mesh->checkGLError();

				// Set up matrix palette
				for (uint16_t idx = 0; idx < mesh->used_bones.size(); ++idx)
				{
					for (uint8_t k = 0; k < 12; ++k)
					{
						this->animation_buffer.matrix_palette_buffer[12 * idx + k] = model->skeleton.transform_buffer[mesh->used_bones[idx]][k];
					}
				}

				// load base uniforms
				mesh->material->syncAnimationBuffer();
				// handle animation
				model->anim_sampler.sampleCurrentFrameUniforms(mesh->material->name_hash, this->animation_buffer.shader_uniform_buffer);
				model->base_anim_sampler.sampleCurrentFrameUniforms(mesh->material->name_hash, this->animation_buffer.shader_uniform_buffer);
				// upload
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

			// Advance animation time
			model->base_anim_sampler.tick();
			model->anim_sampler.tick();
			
			float whole_part;
			this->increment_test = std::modf(this->increment_test, &whole_part);
			(*this->animation_buffer.Time)[0] = this->increment_test;
		}

	}
}

#include "moc_RenderWidget.cpp"
