#pragma once
#include <array>
#include <QOpenGLFunctions>
#include "../../../FileFormats/DSCS/GeomFile/MeshReadWrite.hpp"
#include "OpenGLDSCSMaterial.hpp"



namespace Rendering::DSCS::DataObjects
{
	enum class cgVertexAttribute : uint16_t
	{
		Position = 0,
		BoneWeight = 1,
		Normal = 2,
		Colour = 3,
		WeightedBoneID = 7,
		UV = 8,
		UV2 = 9,
		UV3 = 10,
		Tangent = 14,
		Binormal = 15
	};

	struct OpenGLDSDSVertexAttribute
	{
		cgVertexAttribute attribute_type = cgVertexAttribute::Position;
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
		std::shared_ptr<OpenGLDSCSMaterial> material = nullptr;
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
		//uint32_t normal_buffer_id = 0;
		//std::vector<float> normal_buffer;
		std::vector<OpenGLDSDSVertexAttribute> vertex_attributes;

		VertexInfo editable_vertex_info;
		std::vector<EditableVertex> editable_vertices;

		std::map<uint8_t, GLenum> dtype_map_DSCS_to_GL
		{
			{1, GL_UNSIGNED_BYTE},
			{6, GL_FLOAT},
			{11, GL_HALF_FLOAT}
		};
		std::map<VA_t, cgVertexAttribute> va_map_DSCS_to_GL
		{
			// Cg attributes at https://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_ReferenceManual.pdf
			{ VA_t::Position,       cgVertexAttribute::Position },
			{ VA_t::Normal,         cgVertexAttribute::Normal },
			{ VA_t::Tangent,        cgVertexAttribute::Tangent },
			{ VA_t::Binormal,       cgVertexAttribute::Binormal },
			{ VA_t::UV,             cgVertexAttribute::UV },
			{ VA_t::UV2,            cgVertexAttribute::UV2 },
			{ VA_t::UV3,            cgVertexAttribute::UV3 },
			{ VA_t::Colour,         cgVertexAttribute::Colour },
			{ VA_t::WeightedBoneID, cgVertexAttribute::WeightedBoneID },
			{ VA_t::BoneWeight,     cgVertexAttribute::BoneWeight }
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
