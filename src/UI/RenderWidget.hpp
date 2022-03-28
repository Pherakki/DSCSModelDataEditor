#pragma once

#include <QObject>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QCursor>

#include "Camera.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"
#include "../Rendering/DSCS/Renderer.hpp"
#include "../FileFormats/DSCS/DSCStoOpenGL.hpp"
#include "../Rendering/DSCS/DataObjects/OpenGLDSCSTexture.hpp"
#include "../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "../Utils/Matrix.hpp"


// Need to move most of this into the Renderer at some point, and move the AnimationBuffer code from there into a proper place...


namespace CustomWidgets
{
	class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
	{
		Q_OBJECT;
	public:
		RenderWidget(QWidget* parent);
		~RenderWidget();
		void loadModel(const std::string& path);
		void loadAnim(const std::string& anim_path);
		ModelList_t models;
	signals:
		void vertexShaderTextChanged(const QString& vertexShaderText);
	signals:
		void fragmentShaderTextChanged(const QString& fragmentShaderText);
	private:
		QTimer clock;
		Camera camera;
		std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> shader_backend;
		Rendering::DSCS::AnimationBuffer animation_buffer;
		float targetFrameUpdateTime = 1000 / 30;
		float increment_test = 0.f;
		float aspect_ratio;
		Input::InputHandler input_handler;

		std::unordered_map<std::string, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>> texture_library;
		std::unordered_map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>> shader_library;

		
	protected slots:
		void update();

	protected:
		void initializeGL();
		void resizeGL(int width, int height) { this->aspect_ratio = (float)width / float(height); };
		void paintGL();
		void keyPressEvent    (QKeyEvent* event)   override { event->isAutoRepeat() ? event->ignore() : this->input_handler.registerKey(event->key()); };
		void keyReleaseEvent  (QKeyEvent* event)   override { event->isAutoRepeat() ? event->ignore() : this->input_handler.unregisterKey(event->key());};
		void mousePressEvent  (QMouseEvent* event) override { this->input_handler.registerKey(event->button()); };
		void mouseReleaseEvent(QMouseEvent* event) override { this->input_handler.unregisterKey(event->button()); };
		void registerMousePosition() { this->input_handler.updateMousePosition(QCursor::pos()); }
		void wheelEvent(QWheelEvent* event) override { if (event->delta()) this->camera.mulRadius(-event->delta()); };
	};
}


