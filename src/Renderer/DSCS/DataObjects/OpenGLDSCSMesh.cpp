#include "../../glad/include/glad/glad.h"

#include "OpenGLDSCSMesh.hpp"
#include "../../Utils/Float16.hpp"

namespace Rendering::DSCS::DataObjects
{

	OpenGLDSCSMesh::OpenGLDSCSMesh(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh)
	{
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

		//this->createEditableVertexRepresentation();

		//glGenBuffers(1, &this->normal_buffer_id);
		//glBindBuffer(GL_ARRAY_BUFFER, this->normal_buffer_id);
		//this->checkGLError();
		//auto normal_size = 0.1f;
		//this->normal_buffer = decltype(this->normal_buffer)(3*2*this->editable_vertices.size());
		//for (size_t i = 0; i < this->editable_vertices.size(); ++i)
		//{
		//	for (size_t j = 0; j < 3; ++j)
		//		this->normal_buffer[3*2*i + j] = this->editable_vertices[i].position[j];
		//	for (size_t j = 0; j < 3; ++j)
		//		this->normal_buffer[3*(2*i+1) + j] = this->editable_vertices[i].position[j] + normal_size * this->editable_vertices[i].normal[j];
		//}
		//glBufferData(GL_ARRAY_BUFFER, this->normal_buffer.size(), &this->normal_buffer[0], GL_STATIC_DRAW);
			
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
		this->editable_vertices.clear();
		auto num_vertices = this->mesh.vertices.size() / this->mesh.bytes_per_vertex;
		this->editable_vertices = decltype(this->editable_vertices)(num_vertices);
		size_t cur_offset = 0;
		for (size_t i = 0; i < num_vertices; ++i)
		{
			cur_offset = i * this->mesh.bytes_per_vertex;
			for (const auto& va : this->mesh.vertex_attributes)
			{
				EditableVertex& vertex = this->editable_vertices[i];

				std::array<float, 4> var{};
				auto start = cur_offset = va.vertex_struct_offset;
				switch (dtype_map_DSCS_to_GL[va.data_type])
				{
				case GL_UNSIGNED_BYTE:
					for (size_t i = 0; i < va.num_elements; ++i)
					{
						uint8_t elem;
						memcpy(&elem, &this->mesh.vertices[cur_offset + i * 1], 1);
						var[i] = static_cast<float>(elem);
					}
					break;
				case 0x140B://GL_HALF_FLOAT:
					for (size_t i = 0; i < va.num_elements; ++i)
					{
						uint16_t elem;
						memcpy(&elem, &this->mesh.vertices[cur_offset + i * 2], 2);
						var[i] = fp16tofp32(elem);
					}
					break;
				case GL_FLOAT:
					for (size_t i = 0; i < va.num_elements; ++i)
						memcpy(&var[i], &this->mesh.vertices[cur_offset + i * 4], 4);
					break;
				}

				switch (va_map_DSCS_to_GL[va.attribute_type])
				{
				case cgVertexAttribute::Position:
					vertex.position = { var[0], var[1], var[2] };
					break;
				case cgVertexAttribute::Normal:
					vertex.normal = { var[0], var[1], var[2] };
					break;
				case cgVertexAttribute::Tangent:
					vertex.tangent = var;
					break;
				case cgVertexAttribute::Binormal:
					vertex.binormal = { var[0], var[1], var[2] };
					break;
				case cgVertexAttribute::Colour:
					vertex.color = var;
					break;
				case cgVertexAttribute::UV:
					vertex.uv1 = { var[0], var[1] };
					break;
				case cgVertexAttribute::UV2:
					vertex.uv2 = { var[0], var[1] };
					break;
				case cgVertexAttribute::UV3:
					vertex.uv3 = { var[0], var[1] };
					break;
				case cgVertexAttribute::WeightedBoneID:
					vertex.indices = var;
					break;
				case cgVertexAttribute::BoneWeight:
					vertex.weights = var;
					break;
				}
			}
		}
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

		//glBindBuffer(GL_ARRAY_BUFFER, this->normal_buffer_id);
		//glVertexAttribPointer
		//(
		//	static_cast<GLuint>(cgVertexAttribute::Position), // attribute id
		//	3,                                                // size
		//	GL_FLOAT,                                         // type
		//	GL_FALSE,                                         // normalized
		//	4*3,                                              // stride
		//	0                                                 // array buffer offset
		//);
		//glEnableVertexAttribArray(static_cast<GLuint>(cgVertexAttribute::Position));
		//this->checkGLError();
		//glDrawArrays(GL_LINES, 0, this->normal_buffer.size() / 3);
		//glDisableVertexAttribArray(static_cast<GLuint>(cgVertexAttribute::Position));
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLDSCSMesh::checkGLError()
	{
		GLenum err;
		bool throw_error = false;
		std::stringstream ss;
		while ((err = glGetError()) != GL_NO_ERROR) {
			ss << "ERROR: " << err << '\n';
			throw_error = true;
		}

		if (throw_error)
			throw std::runtime_error(ss.str().c_str());
	}
}