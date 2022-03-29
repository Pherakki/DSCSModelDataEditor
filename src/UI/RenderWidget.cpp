#include "RenderWidget.hpp"

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

		// ViewInverse
		this->animation_buffer.shader_uniform_buffer[0x40] = 1.f;
		this->animation_buffer.shader_uniform_buffer[0x45] = 1.f;
		this->animation_buffer.shader_uniform_buffer[0x47] = -1.f;
		this->animation_buffer.shader_uniform_buffer[0x4A] = 1.f;
		this->animation_buffer.shader_uniform_buffer[0x4A] = -3.f;

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

	void RenderWidget::initializeGL()
	{
		initializeOpenGLFunctions();
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		std::string test_path = "data/pc002";
		std::string anim_path2 = test_path;
		anim_path2 += "_bn01.anim";

		this->loadModel(test_path);
		this->loadAnim(anim_path2);
	}

	void RenderWidget::loadModel(const std::string& path)
	{
		this->models.clear();
		models.emplace(0, FileFormats::DSCS::DSCStoOpenGL(path, this->shader_backend, this->animation_buffer.uniform_dispatch_buffer, this->texture_library, this->shader_library));
		auto& model = models.at(0);
		model.base_anim_sampler.setAnim(model.base_animation);
		model.base_anim_sampler.setSkel(model.skeleton);
	}


	void RenderWidget::loadAnim(const std::string& anim_path)
	{
		auto& model = this->models.at(0);
		FileFormats::DSCS::loadAnimation<false>(model, model.skeleton.getShaderChannelDataBlocks().size(), anim_path);
		model.anim_sampler.setAnim(model.animations[0]);
		model.anim_sampler.setSkel(model.skeleton);
	}

	RenderWidget::~RenderWidget()
	{

	}

	void RenderWidget::setSelectedMesh(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh)
	{
		this->selected_mesh = mesh;
		this->setSelectedMaterial(mesh->material);
	}

	void RenderWidget::setSelectedMaterial(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> material)
	{
		this->selected_material = material;
		emit this->vertexShaderTextChanged(QString::fromStdString(material->shader->vertex_source));
		emit this->fragmentShaderTextChanged(QString::fromStdString(material->shader->fragment_source));
	}

	void RenderWidget::update()
	{
		this->registerMousePosition();

		if (this->input_handler.shouldTranslateCamera())
		{
			auto mdelta = this->input_handler.getMouseDelta();
			this->camera.translate(mdelta.x(), mdelta.y());
		}
		else if (this->input_handler.shouldRotateCamera())
		{
			auto mdelta = this->input_handler.getMouseDelta();
			this->camera.incAltAzi(mdelta.x(), mdelta.y());
		}

		auto view_matrix = makeViewMatrix(this->camera.getPosition(), this->camera.getTarget(), this->camera.getUp());
		auto proj_matrix = perspectiveMatrix(this->camera.fov, this->aspect_ratio, this->camera.zNear, this->camera.zFar);
		auto viewproj = genericMatrixProduct<4, 4, 4>(view_matrix, proj_matrix);

		this->animation_buffer.View->set(view_matrix);
		this->animation_buffer.ViewProj->set(viewproj);
		this->animation_buffer.CameraPosition->set(this->camera.getPosition());
		// Need to calculate ViewInverse here too

		this->increment_test += targetFrameUpdateTime;
		//this->paintGL();
		this->repaint();
	}

	void RenderWidget::paintGL()
	{
		for (auto& kv: this->models)
		{
			auto& model = kv.second;
			model.base_anim_sampler.sampleCurrentFrame(model.skeleton.quat_buffer, model.skeleton.loc_buffer, model.skeleton.scale_buffer, this->animation_buffer.shader_uniform_buffer);
			model.anim_sampler.sampleCurrentFrame(model.skeleton.quat_buffer, model.skeleton.loc_buffer, model.skeleton.scale_buffer);
			model.skeleton.computeTransformBuffer();
			auto& bones = model.skeleton.getBoneDataBlocks();
			for (int j = 0; j < model.meshes.size(); j++)
			{
				auto& mesh = model.meshes[j];
				mesh->checkGLError();

				// Set up matrix palette
				for (uint16_t idx = 0; idx < mesh->used_bones.size(); ++idx)
				{
					for (uint8_t k = 0; k < 12; ++k)
					{
						this->animation_buffer.matrix_palette_buffer[12 * idx + k] = model.skeleton.transform_buffer[mesh->used_bones[idx]][k];
					}
				}

				// load base uniforms
				mesh->material->syncAnimationBuffer();
				// handle animation
				(*this->animation_buffer.Time)[0] = this->increment_test / 10;
				//model.base_anim_sampler.sampleCurrentFrameUniforms(mesh->material->name_hash, this->animation_buffer.shader_uniform_buffer);
				model.anim_sampler.sampleCurrentFrameUniforms(mesh->material->name_hash, this->animation_buffer.shader_uniform_buffer);
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
			model.base_anim_sampler.tick();
			model.anim_sampler.tick();
		}

	}
}

#include "moc_RenderWidget.cpp"
