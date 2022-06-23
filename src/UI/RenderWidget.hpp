#pragma once

#include "../Renderer/DSCS/Renderer.hpp"

#include <QObject>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_1>
#include <QKeyEvent>
#include <QCursor>

#include "InputHandler.hpp"
#include "Types.hpp"
#include "../Renderer/DSCS/AnimationBuffer.hpp"
#include "../Renderer/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "../Utils/Matrix.hpp"

namespace CustomWidgets
{
	//class LampModel : protected QOpenGLFunctions_2_1
	//{
	//private:
	//	GLuint vertex_buffer_id;
	//	GLuint index_buffer_id;
	//	Rendering::DSCS::DataObjects::OpenGLDSCSMaterial material;

	//	inline const static float size = 0.1f;
	//	inline const static std::array<float, 3 * 8> vertices = {
	//		-size, -size, -size,
	//		 size, -size, -size,
	//		-size,  size, -size,
	//		 size,  size, -size,
	//		-size, -size,  size,
	//		 size, -size,  size,
	//		-size,  size,  size,
	//		 size,  size,  size,
	//	};
	//	inline const static std::array<uint16_t, 3 * 12> indices =
	//	{
	//		0, 4, 6,
	//		0, 6, 2,
	//		4, 5, 6,
	//		6, 5, 7,
	//		1, 3, 5,
	//		3, 7, 5,
	//		0, 3, 1,
	//		0, 2, 3
	//	};

	//	std::string vertex_shader_text = "void std_VS ( uniform float4x4 ViewProj : ViewProjection , uniform float4 MatrixPalette[56*3], in float3 Position : POSITION, out float4 vHPosition : POSITION ) {\n"
	//		                             "float4 TmpPosition;\n"
	//	                                 "float4x4 jointMatrix;\n"
	//	                                 "jointMatrix[3] = float4(0.0, 0.0, 0.0, 1.0);\n"
	//	                                 "float3 pos = Position;\n"
	//	                                 "jointMatrix[0] = MatrixPalette[0 + 0];	jointMatrix[1] = MatrixPalette[0 + 1];	jointMatrix[2] = MatrixPalette[0 + 2];	TmpPosition = mul(jointMatrix, pos);;\n"
	//	                                 "vHPosition = mul(ViewProj, TmpPosition);\n"
	//		                             "}\n";

	//	std::string fragment_shader_text = "void std_PS()\n";
	//public:
	//	LampModel(Rendering::ShaderBackends::cgGLShaderBackend& shader_generator,Rendering::DSCS::AnimationBuffer& animation_buffer)
	//		: material{ Rendering::DSCS::DataObjects::OpenGLDSCSMaterial(shader_generator.createShaderProgram(this->vertex_shader_text, this->fragment_shader_text)) }
	//	{
	//		initializeOpenGLFunctions();

	//		glGenBuffers(1, &this->vertex_buffer_id);
	//		glGenBuffers(1, &this->index_buffer_id);

	//		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);

	//		glBufferData(GL_ARRAY_BUFFER, this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);
	//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned short), &this->indices[0], GL_STATIC_DRAW);

	//		this->material.initShaderUniforms(animation_buffer.uniform_dispatch_buffer);
	//	}

	//	~LampModel()
	//	{
	//		glDeleteBuffers(1, &this->vertex_buffer_id);
	//		glDeleteBuffers(1, &this->index_buffer_id);
	//	}

	//	void draw()
	//	{
	//		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);


	//		glVertexAttribPointer
	//		(
	//			static_cast<GLuint>(0),    // attribute id
	//			3,                         // size
	//			GL_FLOAT,                  // type
	//			GL_FALSE,                  // normalized
	//			3*4,                       // stride
	//			reinterpret_cast<void*>(0) // array buffer offset
	//		);
	//		glEnableVertexAttribArray(static_cast<GLuint>(0));
	//		

	//		this->shader->bind();
	//		glDrawElements(
	//			GL_TRIANGLES,
	//			static_cast<GLsizei>(this->indices.size()),
	//			GL_UNSIGNED_SHORT,
	//			reinterpret_cast<void*>(0)
	//		);
	//		this->shader->unbind();

	//		glBindBuffer(GL_ARRAY_BUFFER, 0);
	//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//	}
	//};

	class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_1
	{
		Q_OBJECT;
		typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> ModelPtr;
	public:
		RenderWidget(QWidget* parent);
		~RenderWidget();
		ModelPtr loadModel(const std::string& path);
		void loadAnim(const ModelPtr& model, const std::string& anim_path);
		ModelList_t& models;
		Rendering::DSCS::AnimationBuffer& animation_buffer;
		std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend>& shader_backend;
		std::unordered_map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>>& shader_library;
		Rendering::DSCS::Renderer renderer;
	signals:
		void glInitialised();
	private:
		QTimer clock;
		float targetFrameUpdateTime = 1000 / 30;
		Input::InputHandler input_handler;

		
	protected slots:
		void update();

	protected:
		void initializeGL();
		void resizeGL(int width, int height) { this->renderer.aspect_ratio = (float)width / float(height); };
		void paintGL();
		void keyPressEvent    (QKeyEvent* event)   override { event->isAutoRepeat() ? event->ignore() : this->input_handler.registerKey(event->key()); };
		void keyReleaseEvent  (QKeyEvent* event)   override { event->isAutoRepeat() ? event->ignore() : this->input_handler.unregisterKey(event->key());};
		void mousePressEvent  (QMouseEvent* event) override { this->input_handler.registerKey(event->button()); };
		void mouseReleaseEvent(QMouseEvent* event) override { this->input_handler.unregisterKey(event->button()); };
		void registerMousePosition() { this->input_handler.updateMousePosition(QCursor::pos()); }
		void wheelEvent(QWheelEvent* event) override { if (event->delta()) this->renderer.camera.mulRadius(-event->delta()); };
	};
}


