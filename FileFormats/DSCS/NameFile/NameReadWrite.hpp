#pragma once

#include <string>
#include <vector>

#include <serialisation/ReadWriter.hpp>

using namespace serialisation;

namespace FileFormats::DSCS::NameFile
{
    class NameReadWrite
    {
    private:
        uint32_t bone_name_count = 0;
        uint32_t material_name_count = 0;
        std::vector<uint32_t> bone_name_pointers;
        std::vector<std::string> bone_names;
        std::vector<uint32_t> material_name_pointers;
        std::vector<std::string> material_names;

        template<std::ios_base::openmode openmode_flag> void doReadWrite(const std::string& filepath);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
    public:
        NameReadWrite() {};
        ~NameReadWrite() {};
        void read(const std::string& filepath) { doReadWrite<readmode>(filepath); };
        void write(const std::string& filepath) { doReadWrite<writemode>(filepath); };
        uint32_t getBoneNameCount() { return bone_name_count; };
        uint32_t getMaterialNameCount() { return material_name_count; };
        std::vector<std::string> getBoneNames() { return bone_names; };
        std::vector<std::string> getMaterialNames() { return material_names; };
    };

    // initMemberVectorsIfRequired
    template<>
    void NameReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        bone_name_pointers =     std::vector<uint32_t>(bone_name_count);
        bone_names =             std::vector<std::string>(bone_name_count);
        material_name_pointers = std::vector<uint32_t>(material_name_count);
        material_names =         std::vector<std::string>(material_name_count);
    }
    template<>
    void NameReadWrite::initMemberVectorsIfRequired<writemode>() {}

    // doReadWrite
    template <std::ios_base::openmode openmode_flag>
    void NameReadWrite::doReadWrite(const std::string& filepath)
    {
        ReadWriter<openmode_flag> readWriter(filepath);

        // Read counts
        readWriter.AssertFilePointerIsAt(0);
        readWriter.ReadWriteData<uint32_t, LE>(bone_name_count);
        readWriter.ReadWriteData<uint32_t, LE>(material_name_count);

        // Gives the name vectors and pointer vectors a size if we're in read mode
        initMemberVectorsIfRequired<openmode_flag>();

        // Read the pointers that say where each string in the following section begins
        readWriter.ReadWriteDataArray<uint32_t, LE>(bone_name_pointers);
        readWriter.ReadWriteDataArray<uint32_t, LE>(material_name_pointers);

        // Read bone names
        for (uint32_t i = 0; i < bone_name_count - 1; i++)
        {
            readWriter.AssertFilePointerIsAt(bone_name_pointers[i]);
            uint32_t string_length = bone_name_pointers[i + 1] - bone_name_pointers[i];
            readWriter.ReadWriteAscii(bone_names[i], string_length);
        }
        if (bone_name_count && material_name_count)
            readWriter.ReadWriteAscii(bone_names[bone_name_count - 1], material_name_pointers[0] - bone_name_pointers[bone_name_count - 1]);
        else
        {
            if (bone_name_count && !material_name_count)
                readWriter.ReadWriteAsciiToEOF(bone_names[bone_name_count - 1]);
        }

        // Read material names
        for (uint32_t i = 0; i < material_name_count - 1; i++)
        {
            readWriter.AssertFilePointerIsAt(material_name_pointers[i]);
            uint32_t string_length = material_name_pointers[i + 1] - material_name_pointers[i];
            readWriter.ReadWriteAscii(material_names[i], string_length);
        }
        if (material_name_count)
            readWriter.ReadWriteAsciiToEOF(material_names[material_name_count - 1]);

        readWriter.AssertAtEOF();
    }
}