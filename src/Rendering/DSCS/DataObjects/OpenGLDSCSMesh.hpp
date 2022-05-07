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

	struct VertexInfo
	{
		bool has_normals = false;
		bool has_tangents = false;
		bool has_binormals = false;
		bool has_uv1 = false;
		bool has_uv2 = false;
		bool has_uv3 = false;
		bool has_color = false;
		uint8_t num_weights = 0;
	};

	struct EditableVertex
	{
		std::array<float, 3> position{};
		std::array<float, 3> normal{};
		std::array<float, 4> tangent{};
		std::array<float, 3> binormal{};
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

		void bind();
		void draw();
		void unbind();

		void checkGLError();


	private:
		typedef FileFormats::DSCS::GeomFile::VertexAttributeType VA_t;
		uint32_t vertex_buffer_id = 0;
		uint32_t index_buffer_id = 0;
		std::vector<OpenGLDSDSVertexAttribute> vertex_attributes;

		VertexInfo editable_vertex_info;
		std::vector<EditableVertex> editable_vertices;

		std::map<uint8_t, GLenum> dtype_map_DSCS_to_GL
		{
			{1, GL_UNSIGNED_BYTE},
			{6, GL_FLOAT},
			{11, GL_HALF_FLOAT}
		};
		std::map<VA_t, uint16_t> va_map_DSCS_to_GL
		{
			// Cg attributes at https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_ReferenceManual.pdf
			{ VA_t::Position,        0 },
			{ VA_t::Normal,          2 },
			{ VA_t::Tangent,        14 },
			{ VA_t::Binormal,       15 },
			{ VA_t::UV,              8 },
			{ VA_t::UV2,             9 },
			{ VA_t::UV3,            10 },
			{ VA_t::Colour,          3 },
			{ VA_t::WeightedBoneID,  7 },
			{ VA_t::BoneWeight,      1 }
		};

		void createEditableVertexRepresentation();
		void deleteEditableVertexRepresentation();
		void loadEditableVerticesIntoVertexBuffers();
		void refreshVBO();
		void calculateNormals();
		void calculateTangents(uint8_t which_uv_map);
		void calculateBinormals();
	};

}
