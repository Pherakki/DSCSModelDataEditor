#pragma once

#include <cmath>
#include "../../../../Utils/Vector.hpp"
#include "../../../../serialisation/ReadWriter.hpp"

using namespace serialisation;

namespace FileFormats::DSCS::AnimFile
{
    template<std::ios_base::openmode openmode_flag>
    std::array<float, 4> packUnpackQuaternion(std::array<uint8_t, 6>& bitpack);

    template<std::ios_base::openmode openmode_flag>
    void readWriteQuaternionVector(std::vector<std::array<float, 4>>& slot, ReadWriter<openmode_flag>& read_writer);
    
    struct KeyframeChunkPointers
    {
        uint16_t padding_0x00; // Might be an overflow index?
        uint16_t length;
        uint32_t pointer;
    };

    struct KeyframeChunkCounts
    {
        uint16_t cumulative_count;
        uint16_t increment;
    };

    struct KeyframeChunkHeader
    {
        uint16_t frame_0_rotations_bytecount;
        uint16_t frame_0_locations_bytecount;
        uint16_t frame_0_scales_bytecount;
        uint16_t frame_0_shader_channels_bytecount;

        uint16_t keyframed_rotations_bytecount;
        uint16_t keyframed_locations_bytecount;
        uint16_t keyframed_scales_bytecount;
        uint16_t keyframed_shader_channels_bytecount;
    };

    class KeyframeChunk
    {
    private:
        KeyframeChunkHeader header;

        std::vector<std::array<float, 4>> initial_rotations;
        std::vector<std::array<float, 3>> initial_locations;
        std::vector<std::array<float, 3>> initial_scales;
        std::vector<float>                initial_shader_channel_values;

        std::vector<uint8_t> frame_bitvector;

        std::vector<std::array<float, 4>>  keyframed_rotations;
        std::vector<std::array<float, 3>>  keyframed_locations;
        std::vector<std::array<float, 3>>  keyframed_scales;
        std::vector<float>                 keyframed_shader_channel_values;

    public:
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired(uint16_t frame_bitvector_size);
        template<std::ios_base::openmode openmode_flag> void readWriteInitialFrame(ReadWriter<openmode_flag>& read_write);
        template<std::ios_base::openmode openmode_flag> void readWriteKeyframes(ReadWriter<openmode_flag>& read_write);
        template<std::ios_base::openmode openmode_flag> void doReadWrite(ReadWriter<openmode_flag>& read_writer, uint16_t total_bitvector_size, uint16_t num_frames);

        const auto& getInitialRotations()             const noexcept { return this->initial_rotations; };
        const auto& getInitialLocations()             const noexcept { return this->initial_locations; };
        const auto& getInitialScales()                const noexcept { return this->initial_scales; };
        const auto& getInitialShaderChannelValues()   const noexcept { return this->initial_shader_channel_values; };

        const auto& getKeyframedRotations()           const noexcept { return this->keyframed_rotations; };
        const auto& getKeyframedLocations()           const noexcept { return this->keyframed_locations; };
        const auto& getKeyframedScales()              const noexcept { return this->keyframed_scales; };
        const auto& getKeyframedShaderChannelValues() const noexcept { return this->keyframed_shader_channel_values; };

        const auto& getFrameBitvector()               const noexcept { return this->frame_bitvector; };
    };


    // readWriteInitialFrame
    template<std::ios_base::openmode openmode_flag>
    void KeyframeChunk::readWriteInitialFrame(ReadWriter<openmode_flag>& read_write)
    {
        readWriteQuaternionVector<openmode_flag>(this->initial_rotations, read_write);
        read_write.template readWriteDataVector<std::array<float, 3>, LE>(this->initial_locations);
        read_write.template readWriteDataVector<std::array<float, 3>, LE>(this->initial_scales);
        if (this->header.frame_0_scales_bytecount)
            read_write.readWritePadBytesLeftInIncompleteChunk(read_write.Tell(), 0x04);
        read_write.template readWriteDataVector<float, LE>(this->initial_shader_channel_values);
    }

    // readWriteKeyframes
    template<std::ios_base::openmode openmode_flag>
    void KeyframeChunk::readWriteKeyframes(ReadWriter<openmode_flag>& read_write)
    {
        readWriteQuaternionVector<openmode_flag>(this->keyframed_rotations, read_write);

        read_write.template readWriteDataVector<std::array<float, 3>, LE>(this->keyframed_locations);
        read_write.template readWriteDataVector<std::array<float, 3>, LE>(this->keyframed_scales);
        if (this->header.keyframed_scales_bytecount)
            read_write.readWritePadBytesLeftInIncompleteChunk(read_write.Tell(), 0x04);
        read_write.template readWriteDataVector<float, LE>(this->keyframed_shader_channel_values);
    }

    // doReadWrite
    template <std::ios_base::openmode openmode_flag>
    void KeyframeChunk::doReadWrite(ReadWriter<openmode_flag>& read_writer, uint16_t total_bitvector_size, uint16_t num_frames)
    {
        read_writer.template readWriteData<KeyframeChunkHeader, LE>(this->header);
        this->initMemberVectorsIfRequired<openmode_flag>(total_bitvector_size);
        readWriteInitialFrame<openmode_flag>(read_writer);
        read_writer.template readWriteDataVector<uint8_t, LE>(this->frame_bitvector);
        readWriteKeyframes<openmode_flag>(read_writer);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);
        // subarray
        // int (&array2)[5] = (int (&)[5])(*(array1 + 5));

    }


    struct AnimReadWriteHeader
    {
        uint32_t magic_value;
        float animation_duration;
        float playback_rate;
        uint16_t unused_bone_flags_ptr;
        uint16_t num_bones;
        uint16_t total_frames;
        uint16_t num_keyframe_chunks;
        uint16_t quaternion_precision; // Maybe?

        uint16_t static_bone_rotations_count;
        uint16_t static_bone_locations_count;
        uint16_t static_bone_scales_count;
        uint16_t static_shader_uniform_channels_count;
        uint16_t animated_bone_rotations_count;
        uint16_t animated_bone_locations_count;
        uint16_t animated_bone_scales_count;
        uint16_t animated_shader_uniform_channels_count;
        uint16_t padding_0x26;

        uint32_t unused_channel_bytes;
        uint32_t abs_ptr_bone_mask;
        uint32_t rel_ptr_keyframe_chunks_ptrs;
        uint32_t rel_ptr_keyframe_chunks_counts;
        uint32_t rel_ptr_static_bone_rotations;
        uint32_t rel_ptr_static_bone_locations;
        uint32_t rel_ptr_static_bone_scales;
        uint32_t rel_ptr_static_shader_uniform_channels;

        uint32_t padding_0x48;
        uint32_t padding_0x4C;
        uint32_t padding_0x50;
        uint32_t padding_0x54;
        uint32_t padding_0x58;
        uint32_t padding_0x5C;
    };

    class AnimReadWrite
    {
    private:
        AnimReadWriteHeader header;
        uint32_t shader_channels_count;
        uint32_t abs_ptr_static_bone_rotations;
        uint32_t abs_ptr_static_bone_locations;
        uint32_t abs_ptr_static_bone_scales;
        uint32_t abs_ptr_static_shader_uniform_channels;
        uint32_t abs_ptr_keyframe_chunks_ptrs;
        uint32_t abs_ptr_keyframe_chunks_counts;
        std::vector<uint16_t> static_bone_rotations_idxs;
        std::vector<uint16_t> static_bone_locations_idxs;
        std::vector<uint16_t> static_bone_scales_idxs;
        std::vector<uint16_t> static_shader_channel_idxs;
        std::vector<uint16_t> animated_bone_rotations_idxs;
        std::vector<uint16_t> animated_bone_locations_idxs;
        std::vector<uint16_t> animated_bone_scales_idxs;
        std::vector<uint16_t> animated_shader_channel_idxs;

        std::vector<std::array<float, 4>>  static_bone_rotations;
        std::vector<std::array<float, 3>>  static_bone_locations;
        std::vector<std::array<float, 3>>  static_bone_scales;
        std::vector<float>                 static_shader_channels;
        std::vector<KeyframeChunkPointers> keyframe_chunk_pointers;
        std::vector<KeyframeChunkCounts>   keyframe_chunk_counts;
        std::vector<KeyframeChunk>         keyframe_chunks;
        std::vector<char> unused_bones;
        std::vector<char> unused_shader_channels;

        template<std::ios_base::openmode openmode_flag> void doReadWrite(const std::string&);
        template<std::ios_base::openmode openmode_flag> void readWriteHeader(ReadWriter<openmode_flag>& read_writer);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteBonePoseIndices(ReadWriter<openmode_flag>& read_writer);
        template<std::ios_base::openmode openmode_flag> void readWriteStaticPoseData(ReadWriter<openmode_flag>& read_writer);
        template<std::ios_base::openmode openmode_flag> void readWriteKeyframeChunksLocationData(ReadWriter<openmode_flag>& read_writer);
        template<std::ios_base::openmode openmode_flag> void readWriteUnusedBoneFlags(ReadWriter<openmode_flag>& read_writer);
        template<std::ios_base::openmode openmode_flag> void readWriteKeyframeChunks(ReadWriter<openmode_flag>& read_writer);


    public:
        AnimReadWrite(uint16_t shader_channels_count) { this->shader_channels_count = shader_channels_count; };
        ~AnimReadWrite() {};
        void read(const std::string& filepath) { doReadWrite<readmode>(filepath); };
        void write(const std::string& filepath) { doReadWrite<writemode>(filepath); };

        // Static frames accessors
        const auto& getStaticRotationsCount()      const noexcept { return this->header.static_bone_rotations_count; };
        const auto& getStaticLocationsCount()      const noexcept { return this->header.static_bone_locations_count; };
        const auto& getStaticScalesCount()         const noexcept { return this->header.static_bone_scales_count; };
        const auto& getStaticShaderChannelsCount() const noexcept { return this->header.static_shader_uniform_channels_count; };
        const auto& getStaticRotationIdxs()        const noexcept { return this->static_bone_rotations_idxs; };
        const auto& getStaticLocationIdxs()        const noexcept { return this->static_bone_locations_idxs; };
        const auto& getStaticScaleIdxs()           const noexcept { return this->static_bone_scales_idxs; };
        const auto& getStaticShaderChannelIdxs()   const noexcept { return this->static_shader_channel_idxs; };
        const auto& getStaticRotationValues()      const noexcept { return this->static_bone_rotations; };
        const auto& getStaticLocationValues()      const noexcept { return this->static_bone_locations; };
        const auto& getStaticScaleValues()         const noexcept { return this->static_bone_scales; };
        const auto& getStaticShaderChannelValues() const noexcept { return this->static_shader_channels; };

        // Animated frames accessors
        const auto& getAnimatedRotationsCount()      const noexcept { return this->header.animated_bone_rotations_count; };
        const auto& getAnimatedLocationsCount()      const noexcept { return this->header.animated_bone_locations_count; };
        const auto& getAnimatedScalesCount()         const noexcept { return this->header.animated_bone_scales_count; };
        const auto& getAnimatedShaderChannelsCount() const noexcept { return this->header.animated_shader_uniform_channels_count; };
        const auto& getAnimatedRotationIdxs()        const noexcept { return this->animated_bone_rotations_idxs; };
        const auto& getAnimatedLocationIdxs()        const noexcept { return this->animated_bone_locations_idxs; };
        const auto& getAnimatedScaleIdxs()           const noexcept { return this->animated_bone_scales_idxs; };
        const auto& getAnimatedShaderChannelIdxs()   const noexcept { return this->animated_shader_channel_idxs; };
        const auto& getNumKeyframeChunks()           const noexcept { return this->header.num_keyframe_chunks; };
        const auto& getKeyframeChunks()              const noexcept { return this->keyframe_chunks; };
        const auto& getKeyframeChunkElementCounts()  const noexcept { return this->keyframe_chunk_counts; };

        // Unused frames accessors
        const auto& getUnusedBoneFlags()             const noexcept { return this->unused_bones; };
        const auto& getUnusedShaderChannelFlags()    const noexcept { return this->unused_shader_channels; };

        // Metadata
        const auto& getNumBones()               const noexcept { return this->header.num_bones; };
        const auto& getAnimationLengthSeconds() const noexcept { return this->header.animation_duration; };
        const auto& getFPS()                    const noexcept { return this->header.playback_rate; };
        const auto& getTotalFrames()            const noexcept { return this->header.total_frames; };
    };


    // readWriteHeader
    template <std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteHeader(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(0);
        read_writer.template readWriteData<AnimReadWriteHeader, LE>(this->header);

        assert(this->header.magic_value == 'EA04');
        assert(this->header.quaternion_precision == 16384);
        assert(this->header.padding_0x26 == 0);
        assert(this->header.padding_0x48 == 0);
        assert(this->header.padding_0x4C == 0);
        assert(this->header.padding_0x50 == 0);
        assert(this->header.padding_0x54 == 0);
        assert(this->header.padding_0x58 == 0);
        assert(this->header.padding_0x5C == 0);

        this->abs_ptr_static_bone_rotations = this->header.rel_ptr_static_bone_rotations + offsetof(AnimReadWriteHeader, rel_ptr_static_bone_rotations);
        this->abs_ptr_static_bone_locations = this->header.rel_ptr_static_bone_locations + offsetof(AnimReadWriteHeader, rel_ptr_static_bone_locations);
        this->abs_ptr_static_bone_scales = this->header.rel_ptr_static_bone_scales + offsetof(AnimReadWriteHeader, rel_ptr_static_bone_scales);
        this->abs_ptr_static_shader_uniform_channels = this->header.rel_ptr_static_shader_uniform_channels + offsetof(AnimReadWriteHeader, rel_ptr_static_shader_uniform_channels);
        this->abs_ptr_keyframe_chunks_ptrs = this->header.rel_ptr_keyframe_chunks_ptrs + offsetof(AnimReadWriteHeader, rel_ptr_keyframe_chunks_ptrs);
        this->abs_ptr_keyframe_chunks_counts = this->header.rel_ptr_keyframe_chunks_counts + offsetof(AnimReadWriteHeader, rel_ptr_keyframe_chunks_counts);
    }

    // readWriteBonePoseIndices
    template<std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteBonePoseIndices(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.template readWriteDataVector<uint16_t, LE>(this->static_bone_rotations_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 16, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->static_bone_locations_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->static_bone_scales_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->static_shader_channel_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->shader_channels_count);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->animated_bone_rotations_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->animated_bone_locations_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->animated_bone_scales_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->header.num_bones);

        read_writer.template readWriteDataVector<uint16_t, LE>(this->animated_shader_channel_idxs);
        read_writer.template readWriteBytesLeftInIncompleteChunk<uint16_t, LE>(read_writer.Tell(), 8, this->shader_channels_count);


        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 16);
    }

    // readWriteStaticPoseData
    template<std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteStaticPoseData(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->abs_ptr_static_bone_rotations);
        readWriteQuaternionVector<openmode_flag>(this->static_bone_rotations, read_writer);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);

        read_writer.assertFilePointerIsAt(this->abs_ptr_static_bone_locations);
        read_writer.template readWriteDataVector<std::array<float, 3>, LE>(this->static_bone_locations);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);

        read_writer.assertFilePointerIsAt(this->abs_ptr_static_bone_scales);
        read_writer.template readWriteDataVector<std::array<float, 3>, LE>(this->static_bone_scales);

        read_writer.assertFilePointerIsAt(this->abs_ptr_static_shader_uniform_channels);
        read_writer.template readWriteDataVector<float, LE>(this->static_shader_channels);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);
    }

    template<std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteKeyframeChunksLocationData(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->abs_ptr_keyframe_chunks_ptrs);
        read_writer.template readWriteDataVector<KeyframeChunkPointers, LE>(this->keyframe_chunk_pointers);
        read_writer.assertFilePointerIsAt(this->abs_ptr_keyframe_chunks_counts);
        read_writer.template readWriteDataVector<KeyframeChunkCounts, LE>(this->keyframe_chunk_counts);

        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);
    }


    template<std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteUnusedBoneFlags(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->header.unused_bone_flags_ptr);
        read_writer.template readWriteDataVector<char, LE>(this->unused_bones);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x04);
        read_writer.template readWriteDataVector<char, LE>(this->unused_shader_channels);
        read_writer.readWritePadBytesLeftInIncompleteChunk(read_writer.Tell(), 0x10);
    }


    template<std::ios_base::openmode openmode_flag>
    void AnimReadWrite::readWriteKeyframeChunks(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->header.unused_bone_flags_ptr + this->header.unused_channel_bytes);
        uint16_t frame_bitvector_size = this->header.animated_bone_rotations_count +
            this->header.animated_bone_locations_count +
            this->header.animated_bone_scales_count +
            this->header.animated_shader_uniform_channels_count;
        for (int i = 0; i < this->header.num_keyframe_chunks; ++i)
        {
            auto& keyframe_pointers = this->keyframe_chunk_pointers[i];
            auto& keyframe_counts = this->keyframe_chunk_counts[i];
            auto& keyframe_chunk = this->keyframe_chunks[i];
            // The ( + 8 - 1) makes the integer division round up (probably easier to see the reason why than if I just put 7 here)
            uint16_t total_bitvector_bytes = (frame_bitvector_size * keyframe_counts.increment + 8 - 1) / 8;
            keyframe_chunk.doReadWrite<openmode_flag>(read_writer, total_bitvector_bytes, keyframe_counts.increment);
        }
    }

    // doReadWrite
    template <std::ios_base::openmode openmode_flag>
    void AnimReadWrite::doReadWrite(const std::string& filepath)
    {
        ReadWriter<openmode_flag> read_writer(filepath);

        this->readWriteHeader<openmode_flag>(read_writer);
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteBonePoseIndices(read_writer);
        this->readWriteStaticPoseData(read_writer);
        this->readWriteKeyframeChunksLocationData(read_writer);
        this->readWriteUnusedBoneFlags(read_writer);
        this->readWriteKeyframeChunks(read_writer);

        read_writer.assertAtEOF();
    }


}
