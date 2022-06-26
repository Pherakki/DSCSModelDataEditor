#include "RenderWidget.hpp"

typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
typedef std::shared_ptr<Model> ModelPtr;

namespace CustomWidgets
{
	// Constructor
	RenderWidget::RenderWidget(QWidget* parent) 
		: QOpenGLWidget(parent)
	{
		// Set up the render loop
		connect(&this->clock, &QTimer::timeout, this, &RenderWidget::update);
		connect(&this->clock, &QTimer::timeout, this, [this](){	this->renderer.advTime(targetFrameUpdateTime); /*convert to seconds*/ });
		this->clock.start(1000 * this->targetFrameUpdateTime);
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
		return this->renderer.loadModel(path);
	}


	void RenderWidget::loadAnim(const ModelPtr& model, const std::string& anim_path)
	{
		this->renderer.loadAnim(model, anim_path);
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
			this->renderer.camera.translate(mdelta.x(), mdelta.y());
		}
		else if (this->input_handler.shouldRotateCamera())
		{
			auto& mdelta = this->input_handler.getMouseDelta();
			this->renderer.camera.incAltAzi(mdelta.x(), mdelta.y());
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
		this->renderer.render();
	}
}

#include "moc_RenderWidget.cpp"
