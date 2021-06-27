#pragma once
#include <QOpenGLFunctions>
#include <FileFormats/DSCS/GeomFile/MeshReadWrite.hpp>
#include <Rendering/DataObjects/OpenGLDSCSMaterial.hpp>



namespace Rendering::DataObjects
{
	
	struct OpenGLDSDSVertexAttribute
	{
		uint16_t attribute_type = 0;
		uint16_t num_elements;
		GLenum  data_type;
		uint8_t  always_20;
		uint16_t vertex_struct_offset;
	};
	
	class OpenGLDSCSMesh : protected QOpenGLFunctions
	{
	public:
		OpenGLDSCSMesh(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh);
		~OpenGLDSCSMesh();
		std::shared_ptr<OpenGLDSCSMaterial> material;
		FileFormats::DSCS::GeomFile::MeshReadWrite mesh;

		void bind();
		void draw();
		void unbind();

		void setDSCStoOpenGLVertexAttributes();
		void checkGLError();

	private:
		uint32_t vertex_buffer_id = 0;
		uint32_t index_buffer_id = 0;
		std::vector<OpenGLDSDSVertexAttribute> vertex_attributes;

		std::map<uint8_t, GLenum> dtype_map_DSCS_to_GL
		{
			{1, GL_BYTE},
			{6, GL_FLOAT},
			{11, GL_HALF_FLOAT}
		};
		std::map<uint16_t, uint16_t> va_map_DSCS_to_GL
		{
			{ 1,  0},
			{ 2,  2},
			{ 3, 14},
			{ 4, 15},
			{ 5,  8},
			{ 6,  9},
			{ 7, 10},
			{ 9,  3},
			{10,  7},
			{11,  1}
		};
	};

	OpenGLDSCSMesh::OpenGLDSCSMesh(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh)
	{
		initializeOpenGLFunctions();

		// How do skinning indices and weights work?
		// Handle case where position has 4 components
		this->mesh = mesh;

		glGenBuffers(1, &this->vertex_buffer_id);
		glGenBuffers(1, &this->index_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		this->checkGLError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		this->checkGLError();

		glBufferData(GL_ARRAY_BUFFER, this->mesh.vertices.size(), &this->mesh.vertices[0], GL_STATIC_DRAW);
		this->checkGLError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh.index_buffer.size() * sizeof(unsigned short), &this->mesh.index_buffer[0], GL_STATIC_DRAW);

		for (int i = 0; i < this->mesh.vertex_attributes.size(); i++)
		{
			// Commented out bit should split the 4-component position into a position + weight, when it's done
			auto& va = this->mesh.vertex_attributes[i];
			//if ((uint16_t)va.attribute_type == 1)
			//{
			//	this->vertex_attributes.emplace_back(OpenGLDSDSVertexAttribute
			//		{
			//			va_map_DSCS_to_GL[(uint16_t)va.attribute_type], (uint16_t)(va.num_elements - 1), va.data_type, va.always_20, (uint16_t)(va.vertex_struct_offset - 4)
			//		});
			//}
			this->vertex_attributes.emplace_back(OpenGLDSDSVertexAttribute{ va_map_DSCS_to_GL[(uint16_t)va.attribute_type], va.num_elements, dtype_map_DSCS_to_GL[va.data_type], va.always_20, va.vertex_struct_offset });
		}
	}

	OpenGLDSCSMesh::~OpenGLDSCSMesh()
	{
		glDeleteBuffers(1, &this->vertex_buffer_id);
		this->checkGLError();
		glDeleteBuffers(1, &this->index_buffer_id);
		this->checkGLError();
	}

	void OpenGLDSCSMesh::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		this->checkGLError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		this->checkGLError();
		glBufferData(GL_ARRAY_BUFFER, this->mesh.vertices.size(), &this->mesh.vertices[0], GL_STATIC_DRAW);
		this->checkGLError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh.index_buffer.size() * sizeof(unsigned short), &this->mesh.index_buffer[0], GL_STATIC_DRAW);
		this->checkGLError();

		for (int i = 0; i < 2; i++)
		{
			auto& va = this->vertex_attributes[i];
			if (va.attribute_type != 0)
				continue;
			glVertexAttribPointer
			(
				(GLuint)va.attribute_type,   // attribute id
				va.num_elements,             // size
				GL_FLOAT,                // type
				GL_FALSE,                    // normalized
				this->mesh.bytes_per_vertex, // stride
				(void*)va.vertex_struct_offset // array buffer offset
			);
			checkGLError();
			glEnableVertexAttribArray((GLuint)va.attribute_type);
			checkGLError();
		}
		checkGLError();
	}

	void OpenGLDSCSMesh::draw()
	{
		glDrawElements(
			(GLenum)this->mesh.triangle_data_type,
			this->mesh.index_buffer.size(),
			this->mesh.index_buffer_datatype,
			(void*)0
		);
		this->checkGLError();
	}

	void OpenGLDSCSMesh::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int i = 0; i < this->vertex_attributes.size(); i++)
			glDisableVertexAttribArray(this->vertex_attributes[i].attribute_type);
		this->checkGLError();
	}

	void OpenGLDSCSMesh::checkGLError()
	{
		GLenum err;
		bool throw_error = false;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::stringstream ss;
			ss << "ERROR: " << err << '\n' << std::endl;
			OutputDebugStringA(ss.str().c_str());
			throw_error = true;
		}

		if (throw_error)
			throw std::exception();
	}



}
