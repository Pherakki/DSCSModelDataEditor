#pragma once
#include <array>
#include <QOpenGLFunctions>
#include "../../../FileFormats/DSCS/GeomFile/MeshReadWrite.hpp"
#include "OpenGLDSCSMaterial.hpp"



namespace Rendering::DSCS::DataObjects
{
	
	struct OpenGLDSDSVertexAttribute
	{
		uint16_t attribute_type = 0;
		uint16_t num_elements;
		GLenum  data_type;
		uint8_t  always_20;
		uint16_t vertex_struct_offset;
	};

	struct EditableVertex
	{
		std::array<float, 3> position{};
		std::array<float, 3> normal{};
		std::array<float, 3> tangent{};
		std::array<float, 3> bitangent{};
		std::array<float, 2> uv1{};
		std::array<float, 2> uv2{};
		std::array<float, 2> uv3{};
		std::array<float, 4> color{};
		std::array<float, 4> weights{};
		std::array<float, 4> indices{};
	};
	
	class OpenGLDSCSMesh : protected QOpenGLFunctions
	{
	public:
		OpenGLDSCSMesh(const FileFormats::DSCS::GeomFile::MeshReadWrite& mesh);
		~OpenGLDSCSMesh();
		std::shared_ptr<OpenGLDSCSMaterial> material;
		FileFormats::DSCS::GeomFile::MeshReadWrite mesh;
		std::vector<uint32_t> used_bones;
		std::vector<EditableVertex> editable_vertices;

		void bind();
		void draw();
		void unbind();

		void checkGLError();


	private:
		uint32_t vertex_buffer_id = 0;
		uint32_t index_buffer_id = 0;
		std::vector<OpenGLDSDSVertexAttribute> vertex_attributes;

		std::map<uint8_t, GLenum> dtype_map_DSCS_to_GL
		{
			{1, GL_UNSIGNED_BYTE},
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

		void createEditableVertexRepresentation();
		void deleteEditableVertexRepresentation();
		void loadEditableVerticesIntoVertexBuffers();
	};

}
