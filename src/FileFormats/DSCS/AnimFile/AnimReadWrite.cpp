#include "AnimReadWrite.hpp"

namespace FileFormats::DSCS::AnimFile
{

    // packUnpackQuaternion
    template<>
    std::array<float, 4> packUnpackQuaternion<readmode>(std::array<uint8_t, 6>& bitpack)
    {
        const float invSqrt2 = 0.707107f;

        // This is a bit-packed quaternion
        // Bit-packing goes:
        // [Pad bit] [15 bit unit float] [15 bit unit float] [15 bit unit float] [2 bit index]
        // Need to add a '0' to the front of the 15-bit float bit-representations to make it into a 16-bit
        uint16_t i1 = ((bitpack[0] & 0x7F) << 8) | (bitpack[1] >> 0);
        uint16_t i2 = (bitpack[2] << 7) | (bitpack[3] >> 1);
        uint16_t i3 = ((bitpack[3] & 0x01) << 14) | (bitpack[4] << 6) | (bitpack[5] >> 2);
        uint16_t index = bitpack[5] & 3;

        float f1 = (invSqrt2 * (i1 - 16383)) / 16384;
        float f2 = (invSqrt2 * (i2 - 16383)) / 16384;
        float f3 = (invSqrt2 * (i3 - 16383)) / 16384;

        uint8_t f1_pos = 0 + (index <= 0);
        uint8_t f2_pos = 1 + (index <= 1);
        uint8_t f3_pos = 2 + (index <= 2);

        float f4 = std::sqrt(1 - f1 * f1 - f2 * f2 - f3 * f3);

        std::array<float, 4> quat;
        quat[f1_pos] = f1;
        quat[f2_pos] = f2;
        quat[f3_pos] = f3;
        quat[index] = f4;

        return quat;
    }

    // readWriteQuatertnionVector
    template<>
    void readWriteQuaternionVector<readmode>(std::vector<std::array<float, 4>>& slot, ReadWriter<readmode>& read_writer)
    {
        uint16_t num_elements = slot.size();
        std::vector<std::array<uint8_t, 6>> bitpacked_quats(num_elements);
        read_writer.readWriteDataVector<std::array<uint8_t, 6>, LE>(bitpacked_quats);

        std::vector<std::array<float, 4>> quats(num_elements);
        for (int i = 0; i < num_elements; ++i)
        {
            slot[i] = packUnpackQuaternion<readmode>(bitpacked_quats[i]);
        }
    }


    // initMemberVectorsIfRequired
    template<>
    void KeyframeChunk::initMemberVectorsIfRequired<readmode>(uint16_t frame_bitvector_size)
    {
        this->initial_rotations = std::vector<std::array<float, 4>>(this->header.frame_0_rotations_bytecount / 6);
        this->initial_locations = std::vector<std::array<float, 3>>(this->header.frame_0_locations_bytecount / 12);
        this->initial_scales = std::vector<std::array<float, 3>>(this->header.frame_0_scales_bytecount / 12);
        this->initial_shader_channel_values = std::vector<float>(this->header.frame_0_shader_channels_bytecount / 4);

        this->frame_bitvector = std::vector<uint8_t>(frame_bitvector_size);

        this->keyframed_rotations = std::vector<std::array<float, 4>>(this->header.keyframed_rotations_bytecount / 6);
        this->keyframed_locations = std::vector<std::array<float, 3>>(this->header.keyframed_locations_bytecount / 12);
        this->keyframed_scales = std::vector<std::array<float, 3>>(this->header.keyframed_scales_bytecount / 12);
        this->keyframed_shader_channel_values = std::vector<float>(this->header.keyframed_shader_channels_bytecount / 4);
    }
    template<>
    void KeyframeChunk::initMemberVectorsIfRequired<writemode>(uint16_t frame_bitvector_size) {}

    // initDataVectorsIfRequired
    template<>
    void AnimReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        this->static_bone_rotations_idxs = std::vector<uint16_t>(this->header.static_bone_rotations_count);
        this->static_bone_locations_idxs = std::vector<uint16_t>(this->header.static_bone_locations_count);
        this->static_bone_scales_idxs = std::vector<uint16_t>(this->header.static_bone_scales_count);
        this->static_shader_channel_idxs = std::vector<uint16_t>(this->header.static_shader_uniform_channels_count);

        this->animated_bone_rotations_idxs = std::vector<uint16_t>(this->header.animated_bone_rotations_count);
        this->animated_bone_locations_idxs = std::vector<uint16_t>(this->header.animated_bone_locations_count);
        this->animated_bone_scales_idxs = std::vector<uint16_t>(this->header.animated_bone_scales_count);
        this->animated_shader_channel_idxs = std::vector<uint16_t>(this->header.animated_shader_uniform_channels_count);

        this->static_bone_rotations = std::vector<std::array<float, 4>>(this->header.static_bone_rotations_count);
        this->static_bone_locations = std::vector<std::array<float, 3>>(this->header.static_bone_locations_count);
        this->static_bone_scales = std::vector<std::array<float, 3>>(this->header.static_bone_scales_count);
        this->static_shader_channels = std::vector<float>(this->header.static_shader_uniform_channels_count);

        this->keyframe_chunk_pointers = std::vector<KeyframeChunkPointers>(this->header.num_keyframe_chunks);
        this->keyframe_chunk_counts = std::vector<KeyframeChunkCounts>(this->header.num_keyframe_chunks);
        this->keyframe_chunks = std::vector<KeyframeChunk>(this->header.num_keyframe_chunks);

        if (this->header.unused_channel_bytes)
        {
            this->unused_bones = std::vector<char>(this->header.num_bones);
            this->unused_shader_channels = std::vector<char>(this->shader_channels_count);
        }
        else
        {
            this->unused_bones = std::vector<char>(0);
            this->unused_shader_channels = std::vector<char>(0);
        }
    }
    template<>
    void AnimReadWrite::initMemberVectorsIfRequired<writemode>()
    {}

}