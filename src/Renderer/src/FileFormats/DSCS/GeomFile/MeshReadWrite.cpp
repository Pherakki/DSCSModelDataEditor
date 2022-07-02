#include "MeshReadWrite.hpp"


namespace FileFormats::DSCS::GeomFile
{

    MeshReadWrite::MeshReadWrite(const MeshReadWrite& other)
    {
        // Header Variables
        vertex_data_ptr = other.vertex_data_ptr;
        triangles_ptr = other.triangles_ptr;
        skinning_indices_ptr = other.skinning_indices_ptr;
        vertex_attribute_definitions_ptr = other.vertex_attribute_definitions_ptr;
        num_skinning_indices = other.num_skinning_indices;
        num_vertex_attributes = other.num_vertex_attributes;
        bytes_per_vertex = other.bytes_per_vertex;
        max_skinning_indices_per_vertex = other.max_skinning_indices_per_vertex;
        mesh_flags = other.mesh_flags;
        triangle_data_type = other.triangle_data_type;
        name_hash = other.name_hash;
        material_id = other.material_id;
        num_vertices = other.num_vertices;
        num_elements_in_index_buffer = other.num_elements_in_index_buffer;
        bounding_box_diagonal = other.bounding_box_diagonal;
        mesh_centre = other.mesh_centre;
        bounding_box_lengths = other.bounding_box_lengths;

        // Data Holders
        vertices = other.vertices;
        skinning_indices = other.skinning_indices;
        index_buffer = other.index_buffer;
        vertex_attributes = other.vertex_attributes;
    }

    void swap(MeshReadWrite& first, MeshReadWrite& second)
    {
        using std::swap;
        swap(first.vertex_data_ptr, second.vertex_data_ptr);
        swap(first.triangles_ptr, second.triangles_ptr);
        swap(first.skinning_indices_ptr, second.skinning_indices_ptr);
        swap(first.vertex_attribute_definitions_ptr, second.vertex_attribute_definitions_ptr);
        swap(first.num_skinning_indices, second.num_skinning_indices);
        swap(first.num_vertex_attributes, second.num_vertex_attributes);
        swap(first.bytes_per_vertex, second.bytes_per_vertex);
        swap(first.max_skinning_indices_per_vertex, second.max_skinning_indices_per_vertex);
        swap(first.mesh_flags, second.mesh_flags);
        swap(first.triangle_data_type, second.triangle_data_type);
        swap(first.name_hash, second.name_hash);
        swap(first.material_id, second.material_id);
        swap(first.bounding_box_diagonal, second.bounding_box_diagonal);
        swap(first.mesh_centre, second.mesh_centre);
        swap(first.bounding_box_lengths, second.bounding_box_lengths);

        swap(first.vertices, second.vertices);
        swap(first.skinning_indices, second.skinning_indices);
        swap(first.index_buffer, second.index_buffer);
        swap(first.vertex_attributes, second.vertex_attributes);
    }

    // initMemberVectorsIfRequired
    template<>
    void MeshReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        this->vertices = std::vector<uint8_t>(this->num_vertices * this->bytes_per_vertex);
        this->skinning_indices = std::vector<uint32_t>(this->num_skinning_indices);
        this->index_buffer = std::vector<uint16_t>(this->num_elements_in_index_buffer);
        this->vertex_attributes = std::vector<VertexAttribute>(this->num_vertex_attributes);
    }
    template<>
    void MeshReadWrite::initMemberVectorsIfRequired<writemode>() {}
}