#pragma once

#include <FileFormats/DSCS/GeomFile/MeshReadWrite.hpp>
#include <Cg/cgGL.h>
#include <QtGui/qopenglfunctions.h>


namespace Rendering::DataObjects
{
	class BindableVertices : protected QOpenGLFunctions
	{
	public:
		BindableVertices(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh);
		~BindableVertices();
		FileFormats::DSCS::GeomFile::MeshReadWrite mesh;

		void draw();
		void bind();
		void unbind();
	private:
		unsigned int vertex_buffer_id;
		unsigned int index_buffer_id;
		void checkGLError();


	};

	void BindableVertices::checkGLError()
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



	BindableVertices::BindableVertices(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh)
	{
		initializeOpenGLFunctions();
		this->checkGLError();
		glGenBuffers(1, &this->vertex_buffer_id);
		this->checkGLError();
		glGenBuffers(1, &this->index_buffer_id);
		this->checkGLError();

		this->mesh = mesh;
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		this->checkGLError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		this->checkGLError();

	}

	BindableVertices::~BindableVertices()
	{
		glDeleteBuffers(1, &this->vertex_buffer_id);
		glDeleteBuffers(1, &this->index_buffer_id);
	}

	void BindableVertices::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		this->checkGLError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		this->checkGLError();
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
		this->checkGLError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer.size() * sizeof(unsigned short), &mesh.index_buffer[0], GL_STATIC_DRAW);
		this->checkGLError();

		for (int i = 0; i < this->mesh.num_vertex_attributes; i++)
		{

			auto& va = this->mesh.vertex_attributes[i]; 
			GLenum dtype;
			switch (va.data_type)
			{
				case 1: dtype = GL_BYTE; break;
				case 6: dtype = GL_FLOAT; break;
				case 11: dtype = GL_HALF_FLOAT; break;
				default: throw std::exception();
			}
			
			if ((int)va.attribute_type == 1)

				glVertexAttribPointer(
					0,							 // Position ID
					va.num_elements,             // size
					dtype,                       // type
					GL_FALSE,                    // normalised
					this->mesh.bytes_per_vertex, // stride
					(void*)0                     // offset
				);

			this->checkGLError();
			
			this->checkGLError();
		}
		glEnableVertexAttribArray(0);



	}

	void BindableVertices::draw()
	{
		glDrawElements(
			(GLenum)this->mesh.triangle_data_type,
			this->mesh.index_buffer.size(),
			this->mesh.index_buffer_datatype,
			(void*)0
		);
		this->checkGLError();
	}

	void BindableVertices::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int i = 0; i < this->mesh.num_vertex_attributes; i++)
		{
			glDisableVertexAttribArray((GLuint)this->mesh.vertex_attributes[i].attribute_type);
		}
	}
}