#include "SkelReadWrite.hpp"

namespace FileFormats::DSCS::SkelFile
{
    // initMemberVectorsIfRequired
    template<>
    void SkelReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        this->bone_heirarchy = std::vector<std::array<int16_t, 8>>(num_bone_hierarchy_data_rows);
        this->rest_pose_data = std::vector<std::array<float, 12>>(num_bones);
        this->bone_parents = std::vector<int16_t>(num_bones);
        this->shader_channel_flags = std::vector<uint8_t>(num_shader_channels);
        this->bone_name_hashes = std::vector<uint32_t>(num_bones);
        this->shader_channel_buffer_offsets = std::vector<uint32_t>(num_shader_channels);
        this->shader_channel_name_hashes = std::vector<uint32_t>(num_shader_channels);
    }
    template<>
    void SkelReadWrite::initMemberVectorsIfRequired<writemode>() {}
}