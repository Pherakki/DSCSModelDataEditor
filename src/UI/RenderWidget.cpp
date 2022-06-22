#include "RenderWidget.hpp"

typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
typedef std::shared_ptr<Model> ModelPtr;

namespace CustomWidgets
{
	// Constructor
	RenderWidget::RenderWidget(QWidget* parent) 
		: QOpenGLWidget(parent)
		, animation_buffer{renderer.animation_buffer}
		, shader_backend{ renderer.shader_backend }
		, shader_library{ renderer.shader_library }
		, models{ renderer.models }
		, camera{ renderer.camera }
	{
		// Set up the render loop
		connect(&this->clock, &QTimer::timeout, this, &RenderWidget::update);
		connect(&this->clock, &QTimer::timeout, this, [this](){	this->increment_test += 0.001 * targetFrameUpdateTime; /*convert to seconds*/ });
		this->clock.start(this->targetFrameUpdateTime);
		this->setFocusPolicy(Qt::StrongFocus);
	}

	void RenderWidget::initializeGL()
	{
		this->renderer.initRenderer();
		initializeOpenGLFunctions();

		std::cout << "Render Widget Init" << std::endl;
		emit this->glInitialised();
	}

	ModelPtr RenderWidget::loadModel(const std::string& path)
	{
		//this->models.clear();
		auto id = models.size();
		models.emplace(id, std::make_shared<Model>(FileFormats::DSCS::DSCStoOpenGL(path, this->shader_backend, this->animation_buffer.uniform_dispatch_buffer, this->texture_library, this->shader_library)));
		auto& model = models.at(id);
		model->base_anim_sampler.setAnim(model->base_animation);
		model->base_anim_sampler.setSkel(model->skeleton);

		return model;
	}


	void RenderWidget::loadAnim(const ModelPtr& model, const std::string& anim_path)
	{
		//auto& model = this->models.at(0);
		FileFormats::DSCS::loadAnimation<false>(*model, model->skeleton.getShaderChannelDataBlocks().size(), anim_path);
		model->anim_sampler_a.setAnim(model->animations[0]);
		model->anim_sampler_a.setSkel(model->skeleton);
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

		if (this->input_handler.toggleAnimation())
		{
			// Toggle animation
		}

		this->renderer.recalculateGlobalUniforms();

		//std::cout << this->increment_test << std::endl;
		this->repaint();
	}

	void RenderWidget::paintGL()
	{
		this->renderer.refreshRenderSettings();
		for (auto& kv: this->models)
		{
			auto& model = kv.second;
			auto& skeleton = model->skeleton;
			// Calculate the model's skeleton position for this frame
			model->sampleSkeletalAnimation();
			auto& bones = model->skeleton.getBoneDataBlocks();
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

			// Advance animation time
			model->tickSamplers();
		}

		// Rotate the light in a circle, just to test the illumination render
		float whole_part;
		//auto angle = 2*3.14*std::modf(this->increment_test/2, &whole_part);
		//this->animation_buffer.DirLamp01Dir->set({ 2.f * std::sinf(angle), 0.5f, 2.f * std::cosf(angle)});

		//this->increment_test = std::modf(this->increment_test, &whole_part);
		(*this->animation_buffer.Time)[0] = this->increment_test;

	}
}

#include "moc_RenderWidget.cpp"
