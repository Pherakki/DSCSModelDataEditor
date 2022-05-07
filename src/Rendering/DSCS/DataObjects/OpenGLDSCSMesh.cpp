#include "OpenGLDSCSMesh.hpp"
#include "Utils/Float16.hpp"

namespace Rendering::DSCS::DataObjects
{

	OpenGLDSCSMesh::OpenGLDSCSMesh(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh)
	{
		initializeOpenGLFunctions();

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
		this->checkGLError();

		for (const auto& va : this->mesh.vertex_attributes)
		{
			this->vertex_attributes.emplace_back(OpenGLDSDSVertexAttribute{ va_map_DSCS_to_GL[va.attribute_type], va.num_elements, dtype_map_DSCS_to_GL[va.data_type], va.always_20, va.vertex_struct_offset });
		}
	}

	OpenGLDSCSMesh::~OpenGLDSCSMesh()
	{
		glDeleteBuffers(1, &this->vertex_buffer_id);
		this->checkGLError();
		glDeleteBuffers(1, &this->index_buffer_id);
		this->checkGLError();
	}

	void OpenGLDSCSMesh::refreshVBO()
	{
		glBufferData(GL_ARRAY_BUFFER, this->mesh.vertices.size(), &this->mesh.vertices[0], GL_STATIC_DRAW);
		this->checkGLError();
	}

	// Will create a vector of Vertex items that can be easily talked to
	void OpenGLDSCSMesh::createEditableVertexRepresentation()
	{

	}

	// Delete the editable vertices
	void OpenGLDSCSMesh::deleteEditableVertexRepresentation()
	{
		// Clear and shrink the editable vertices back to 0 capacity
		this->editable_vertices.clear();
		this->editable_vertices = decltype(this->editable_vertices){};
	}

	// Replace the model's vertex data/attributes with those in the editable format
	void OpenGLDSCSMesh::loadEditableVerticesIntoVertexBuffers()
	{

	}

	void OpenGLDSCSMesh::calculateTangents(uint8_t which_uv_map)
	{

	}

	void OpenGLDSCSMesh::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_id);
		this->checkGLError();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->index_buffer_id);
		this->checkGLError();

		for (const auto& va : this->vertex_attributes)
		{
			glVertexAttribPointer
			(
				static_cast<GLuint>(va.attribute_type),          // attribute id
				va.num_elements,                                 // size
				va.data_type,                                    // type
				GL_FALSE,                                        // normalized
				this->mesh.bytes_per_vertex,                     // stride
				reinterpret_cast<void*>(va.vertex_struct_offset) // array buffer offset
			);
			checkGLError();
			glEnableVertexAttribArray(static_cast<GLuint>(va.attribute_type));
			checkGLError();
		}
		checkGLError();
	}

	void OpenGLDSCSMesh::draw()
	{
		this->checkGLError();
		glDrawElements(
			(GLenum)this->mesh.triangle_data_type,
			(GLsizei)this->mesh.index_buffer.size(),
			(GLenum)this->mesh.index_buffer_datatype,
			(void*)0
		);
		this->checkGLError();
	}

	void OpenGLDSCSMesh::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (int i = 0; i < this->vertex_attributes.size(); i++)
			glDisableVertexAttribArray(static_cast<GLuint>(this->vertex_attributes[i].attribute_type));
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