#pragma once

#include <string>
#include <vector>

#include "../../../serialisation/ReadWriter.hpp"
#include "../../../serialisation/Macros.hpp"

using namespace serialisation;

namespace FileFormats::DSCS::SkelFile
{

    class SkelReadWrite
    {
    private:
        const std::string magic_value = "20SE";
        uint32_t          total_bytes = 0;
        const uint32_t    padding_0x08 = 0;
        uint32_t          num_bytes_in_name_hashes_section = 0;
        uint16_t          num_bones = 0;
        uint16_t          num_shader_channels = 0;
        uint16_t          num_bone_hierarchy_data_rows = 0;
        const uint16_t    padding_0x16 = 0;
        uint32_t          rel_ptr_to_rest_pose_data = 0;
        uint32_t          rel_ptr_to_bone_parents = 0;
        uint32_t          rel_ptr_to_bone_name_hashes = 0;
        uint32_t          rel_ptr_to_shader_channel_buffer_offsets = 0;
        uint32_t          rel_ptr_to_shader_channel_name_hashes = 0;
        uint32_t          rel_ptr_to_uv_data_1 = 0;
        const uint32_t    padding_0x30 = 0;
        const uint32_t    padding_0x34 = 0;
        const uint32_t    padding_0x38 = 0;
        const uint32_t    padding_0x3C = 0;

        uint32_t getAbsPtrToRestPoseData() { return rel_ptr_to_rest_pose_data + 0x18; };
        uint32_t getAbsPtrToBoneParents() { return rel_ptr_to_bone_parents + 0x1C; };
        uint32_t getAbsPtrToBoneNameHashes() { return rel_ptr_to_bone_name_hashes + 0x20; };
        uint32_t getAbsPtrToShaderChannelBufferOffsets() { return rel_ptr_to_shader_channel_buffer_offsets + 0x24; };
        uint32_t getAbsPtrToShaderChannelNameHashes() { return rel_ptr_to_shader_channel_name_hashes + 0x28; };
        uint32_t getAbsPtrToUVData1() { return rel_ptr_to_uv_data_1 + 0x2C; };

        std::vector<std::array<int16_t, 8>> bone_heirarchy = {};
        std::vector<std::array<float, 12>>  rest_pose_data = {};
        std::vector<int16_t>                bone_parents = {};
        std::vector<uint8_t>                shader_channel_flags = {};
        std::vector<uint32_t>               bone_name_hashes = {};
        std::vector<uint32_t>               shader_channel_buffer_offsets = {};
        std::vector<uint32_t>               shader_channel_name_hashes = {};

        template<std::ios_base::openmode openmode_flag> void readWriteHeader(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteBoneHierarchy(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteRestPoseData(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteParentBones(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteUVData1(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteBoneNameHashes(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteShaderChannelBufferOffsets(ReadWriter<openmode_flag>&);
        template <std::ios_base::openmode openmode_flag> void readWriteShaderChannelNameHashes(ReadWriter<openmode_flag>&);

        template<std::ios_base::openmode openmode_flag> void doReadWrite(const std::string&);
    public:
        SkelReadWrite() {};
        ~SkelReadWrite() {};
        void read(const std::string& filepath) { doReadWrite<readmode>(filepath); };
        void write(const std::string& filepath) { doReadWrite<writemode>(filepath); };

        const auto& getNumBones()                   { return this->num_bones; }
        const auto& getRestPoseData()               { return this->rest_pose_data; };
        const auto& getBoneParents()                { return this->bone_parents; };
        const auto& getNumShaderChannels()          { return this->num_shader_channels; };
        const auto& getShaderChannelFlags()         { return this->shader_channel_flags; };
        const auto& getShaderChannelObjectHashes()  { return this->shader_channel_name_hashes; };
        const auto& getShaderChannelBufferOffsets() { return this->shader_channel_buffer_offsets; };

    };


    // readWriteHeader
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteHeader(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.readWriteConstAscii(this->VALUE_AND_NAME_OF(magic_value));
        read_writer.readWriteData<uint32_t, LE>(this->total_bytes);
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x08));
        read_writer.readWriteData<uint32_t, LE>(this->num_bytes_in_name_hashes_section);
        read_writer.readWriteData<uint16_t, LE>(this->num_bones);
        read_writer.readWriteData<uint16_t, LE>(this->num_shader_channels);
        read_writer.readWriteData<uint16_t, LE>(this->num_bone_hierarchy_data_rows);
        read_writer.readWriteConstData<uint16_t, LE>(this->VALUE_AND_NAME_OF(padding_0x16));
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_rest_pose_data);
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_bone_parents);
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_bone_name_hashes);
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_shader_channel_buffer_offsets);
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_shader_channel_name_hashes);
        read_writer.readWriteData<uint32_t, LE>(this->rel_ptr_to_uv_data_1);
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x30));
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x34));
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x38));
        read_writer.readWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x3C));
    }



    // readWriteBoneHierarchy
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteBoneHierarchy(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.readWriteDataVector<std::array<int16_t, 8>, LE>(this->bone_heirarchy);
    }

    // readWriteBoneHierarchy
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteRestPoseData(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToRestPoseData());
        read_writer.readWriteDataVector<std::array<float, 12>, LE>(this->rest_pose_data);
    }

    // readWriteParentBones
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteParentBones(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToBoneParents());
        read_writer.readWriteDataVector<int16_t, LE>(this->bone_parents);
    }

    // readWriteUVData1
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteUVData1(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToUVData1());
        read_writer.readWriteDataVector<uint8_t, LE >(this->shader_channel_flags);
    }

    // readWriteBoneNameHashes
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteBoneNameHashes(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToBoneNameHashes());
        read_writer.readWriteDataVector<uint32_t, LE >(this->bone_name_hashes);
    }

    // readWriteShaderChannelBufferOffsets
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteShaderChannelBufferOffsets(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToShaderChannelBufferOffsets());
        read_writer.readWriteDataVector<uint32_t, LE >(this->shader_channel_buffer_offsets);
    }

    // readWriteShaderChannelNameHashes
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::readWriteShaderChannelNameHashes(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.assertFilePointerIsAt(this->getAbsPtrToShaderChannelNameHashes());
        read_writer.readWriteDataVector<uint32_t, LE >(this->shader_channel_name_hashes);
    }


    // doReadWrite
    template <std::ios_base::openmode openmode_flag>
    void SkelReadWrite::doReadWrite(const std::string& filepath)
    {
        ReadWriter<openmode_flag> read_writer(filepath);

        this->readWriteHeader<openmode_flag>(read_writer);
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteBoneHierarchy(read_writer);
        this->readWriteRestPoseData(read_writer);
        this->readWriteParentBones(read_writer);
        this->readWriteUVData1(read_writer);
        size_t pos = read_writer.Tell();
        read_writer.readWritePadBytesLeftInIncompleteChunk(pos, 16);
        this->readWriteBoneNameHashes(read_writer);
        this->readWriteShaderChannelBufferOffsets(read_writer);
        this->readWriteShaderChannelNameHashes(read_writer);
        pos = read_writer.Tell();
        read_writer.readWritePadBytesLeftInIncompleteChunk(pos - this->num_bytes_in_name_hashes_section, 16);

        read_writer.assertAtEOF();
    }
}
