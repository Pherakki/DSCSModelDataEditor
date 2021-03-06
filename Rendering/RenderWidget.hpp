#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <Rendering/ShaderBackends/cgGL/cgGLShaderBackend.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FileFormats/DSCS/GeomFile/GeomReadWrite.hpp>
#include <Rendering/DataObjects/BindableVertices.hpp>
#include <Rendering/DataObjects/OpenGLDSCSModel.hpp>
#include <FileFormats/DSCS/DSCStoOpenGL.hpp>

static glm::mat4 make_MVP(float width, float height, glm::vec3 cameraPosition, glm::vec3 cameraTarget)
{
	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		cameraPosition, // Camera is at (4,3,3), in World Space
		cameraTarget, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

	return mvp;
}

static std::array<float, 16> make_MVP_float(float width, float height, glm::vec3 cameraPosition, glm::vec3 cameraTarget)
{
	glm::mat4 mvp = make_MVP(width, height, cameraPosition, cameraTarget);
	std::array<float, 16> mvp_float = std::array<float, 16> { mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3],
							                                  mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3],
									                          mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3],
									                          mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3] };
	return mvp_float;
}


namespace CustomWidgets
{
	class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
	{
	public:
		RenderWidget(QWidget* parent);
		~RenderWidget();
		void initializeGL();
		void resizeGL(int width, int height) {};
		void paintGL();
		void teardownGL() {};
	private:
		std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend> shader_backend;

		// Change the key from a string to an int later
		std::map<std::string, Rendering::DataObjects::OpenGLDSCSModel> models;
		std::map<uint32_t, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSMaterial>> material_library;
		std::map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>> shader_library;
	};

	// Constructor
	RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
	{
		this->shader_backend = std::make_unique<Rendering::ShaderBackends::cgGLShaderBackend>();
	}

	RenderWidget::~RenderWidget()
	{
	
	}

	void RenderWidget::initializeGL()
	{
		initializeOpenGLFunctions();
		std::filesystem::path filepath = path/to/test/model;

		this->models["model"] = FileFormats::DSCS::DSCStoOpenGL(filepath,
																this->shader_backend, material_library, shader_library);

		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderWidget::paintGL()
	{
		glm::vec3 m_cameraPosition = glm::vec3(0, 1, 3);
		glm::vec3 m_cameraTarget = glm::vec3(0, 1, 0);

		std::array<float, 16> m_mvp = make_MVP_float(this->width(), this->height(), m_cameraPosition, m_cameraTarget);

		for (int i = 0; i < models.size(); i++)
		{
			auto& model = models["model"];
			for (int j = 0; j < model.meshes.size(); j++)
			{
				auto& mesh = model.meshes[j];
				mesh->material->bind();
				mesh->material->shader->setMVP(m_mvp);
				this->shader_backend->checkForCgError("Setting MVP...");
				mesh->bind();
				mesh->draw();
				mesh->unbind();
				mesh->material->unbind();

				mesh->checkGLError();
				this->shader_backend->checkForCgError("Finishing unbind...");
			}
		}

	}

}
