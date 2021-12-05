#pragma once

#include <array>
#include <vector>
#include "../../../serialisation/Macros.hpp"
#include "../../../serialisation/ReadWriter.hpp"

#include "GeomForwardDeclarations.hpp"

using namespace serialisation;

namespace FileFormats::DSCS::GeomFile
{
    enum class VertexAttributeType : uint16_t
    {
        Position = 1,
        Normal = 2,
        Tangent = 3,
        Binormal = 4,
        UV = 5,
        UV2 = 6,
        UV3 = 7,
        Colour = 9,
        WeightedBoneID = 10,
        BoneWeight = 11
    };

    struct VertexAttribute
    {
        VertexAttributeType attribute_type = VertexAttributeType::Position;
        uint16_t num_elements;
        uint8_t  data_type;
        uint8_t  always_20;
        uint16_t vertex_struct_offset;
    };

    enum class TriangleDataType : uint16_t
    {
        Triangles = 4,
        TriangleStrip = 5
    };
    
    class MeshReadWrite
    {
    private:
        // RW Methods
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteVertices(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteSkinningIndices(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteIndexBuffer(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteVertexAttributes(ReadWriter<openmode_flag>&);

        template<std::ios_base::openmode openmode_flag> void readWriteHeader(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteBody(ReadWriter<openmode_flag>&);

    public:
        // Header variables
        uint64_t vertex_data_ptr = 0;                   // 0x00
        uint64_t triangles_ptr = 0;                     // 0x08
        uint64_t skinning_indices_ptr = 0;              // 0x10
        const uint64_t padding_0x18 = 0;                // 0x18

        uint64_t vertex_attribute_definitions_ptr = 0;  // 0x20
        uint16_t num_skinning_indices = 0;              // 0x28
        uint16_t num_vertex_attributes = 0;             // 0x2A
        uint16_t bytes_per_vertex = 0;                  // 0x2C
        const uint16_t index_buffer_datatype = 0x1403;  // 0x2E

        uint8_t max_skinning_indices_per_vertex = 0;    // 0x30
        uint8_t mesh_flags = 0;                       // 0x31
        TriangleDataType triangle_data_type;            // 0x32
        uint32_t name_hash = 0;                         // 0x34
        uint32_t material_id = 0;                       // 0x38
        uint32_t num_vertices = 0;                      // 0x3C

        uint32_t num_elements_in_index_buffer = 0;      // 0x40
        const uint32_t padding_0x44 = 0;                // 0x44
        const uint32_t padding_0x48 = 0;                // 0x48
        float bounding_box_diagonal = 0;                // 0x4C

        std::array<float, 3> mesh_centre = {};          // 0x50
        std::array<float, 3> bounding_box_lengths = {}; // 0x5C

        // Data Holders
        std::vector<uint8_t> vertices = {};
        std::vector<uint32_t> skinning_indices = {};
        std::vector<uint16_t> index_buffer = {};
        std::vector<VertexAttribute> vertex_attributes = {};

        MeshReadWrite() {};
        ~MeshReadWrite() {};
        friend void swap(MeshReadWrite& first, MeshReadWrite& second);
        MeshReadWrite(const MeshReadWrite&);
        MeshReadWrite& operator=(MeshReadWrite other)
        {
            swap(*this, other);
            return *this;
        }
        MeshReadWrite(MeshReadWrite&& other) noexcept
            : MeshReadWrite()
        {
            swap(*this, other);
        }

        // Friends
        friend class GeomReadWrite;
        friend class DSCStoOpenGL;

	};




    // **************************************** //
    // **************************************** //
    // *** PRIVATE FUNCTION IMPLEMENTATIONS *** //
    // **************************************** //
    // **************************************** //

    //readWriteHeader
    template<std::ios_base::openmode openmode_flag> 
    void MeshReadWrite::readWriteHeader(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.readWriteData<uint64_t, LE>(this->vertex_data_ptr);
        read_writer.readWriteData<uint64_t, LE>(this->triangles_ptr);
        read_writer.readWriteData<uint64_t, LE>(this->skinning_indices_ptr);
        read_writer.readWriteConstData<uint64_t, LE>(this->VALUE_AND_NAME_OF(padding_0x18));

        read_writer.readWriteData<uint64_t, LE>(this->vertex_attribute_definitions_ptr);
        read_writer.readWriteData<uint16_t, LE>(this->num_skinning_indices);
        read_writer.readWriteData<uint16_t, LE>(this->num_vertex_attributes);
        read_writer.readWriteData<uint16_t, LE>(this->bytes_per_vertex);
        read_writer.readWriteConstData<uint16_t, LE>(this->VALUE_AND_NAME_OF(index_buffer_datatype));

        read_writer.readWriteData<uint8_t, LE>(this->max_skinning_indices_per_vertex);
        read_writer.readWriteData<uint8_t, LE>(this->mesh_flags);
        read_writer.readWriteData<TriangleDataType, LE>(this->triangle_data_type);
        read_writer.readWriteData<uint32_t, LE>(this->name_hash);
        read_writer.readWriteData<uint32_t, LE>(this->material_id);
        read_writer.readWriteData<uint32_t, LE>(this->num_vertices);

        read_writer.readWriteData<uint32_t, LE>(this->num_elements_in_index_buffer);
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x44));
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x48));
        read_writer.readWriteData<float, LE>(this->bounding_box_diagonal);

        read_writer.readWriteData<std::array<float, 3>, LE>(this->mesh_centre);
        read_writer.readWriteData<std::array<float, 3>, LE>(this->bounding_box_lengths);
    }



    template<std::ios_base::openmode openmode_flag> 
    void MeshReadWrite::readWriteVertices(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->vertex_data_ptr);
        read_writer.readWriteDataVector<uint8_t, LE>(this->vertices);
    }

    template<std::ios_base::openmode openmode_flag> 
    void MeshReadWrite::readWriteSkinningIndices(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->skinning_indices_ptr);
        read_writer.readWriteDataVector<uint32_t, LE>(this->skinning_indices);
    }

    template<std::ios_base::openmode openmode_flag>
    void MeshReadWrite::readWriteIndexBuffer(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->triangles_ptr);
        read_writer.readWriteDataVector<uint16_t, LE>(this->index_buffer);
    }

    template<std::ios_base::openmode openmode_flag>
    void MeshReadWrite::readWriteVertexAttributes(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->vertex_attribute_definitions_ptr);
        read_writer.readWriteDataVector<VertexAttribute, LE>(this->vertex_attributes);
    }

    //readWriteBody
    template<std::ios_base::openmode openmode_flag> 
    void MeshReadWrite::readWriteBody(ReadWriter<openmode_flag>& read_writer)
    {
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteVertices<openmode_flag>(read_writer);
        this->readWriteSkinningIndices<openmode_flag>(read_writer);
        this->readWriteIndexBuffer<openmode_flag>(read_writer);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 4);
        this->readWriteVertexAttributes<openmode_flag>(read_writer);
    }
}