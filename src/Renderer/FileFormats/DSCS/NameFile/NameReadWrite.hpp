#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "../../../../serialisation/ReadWriter.hpp"

using namespace serialisation;

namespace FileFormats::DSCS::NameFile
{
    class NameReadWrite
    {
    private:
        uint32_t                 bone_name_count = 0;
        uint32_t                 material_name_count = 0;
        std::vector<uint32_t>    bone_name_pointers;
        std::vector<std::string> bone_names;
        std::vector<uint32_t>    material_name_pointers;
        std::vector<std::string> material_names;

        template<std::ios_base::openmode openmode_flag> void doReadWrite(const std::string& filepath);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void checkThatThisMightActuallyBeANameFileBecauseThereIsNoMagicValue(const std::string& filepath);
    public:
        NameReadWrite() {};
        ~NameReadWrite() {};
        void                            read(const std::string& filepath)  { doReadWrite<readmode>(filepath); };
        void                            write(const std::string& filepath) { doReadWrite<writemode>(filepath); };
        const uint32_t&                 getBoneNameCount()                 { return bone_name_count; };
        const uint32_t&                 getMaterialNameCount()             { return material_name_count; };
        const std::vector<std::string>& getBoneNames()                     { return bone_names; };
        const std::vector<std::string>& getMaterialNames()                 { return material_names; };
    };



    // doReadWrite
    template <std::ios_base::openmode openmode_flag>
    void NameReadWrite::doReadWrite(const std::string& filepath)
    {
        ReadWriter<openmode_flag> readWriter(filepath);

        // Read counts
        readWriter.assertFilePointerIsAt(0);
        readWriter.template readWriteData<uint32_t, LE>(bone_name_count);
        readWriter.template readWriteData<uint32_t, LE>(material_name_count);
        this->checkThatThisMightActuallyBeANameFileBecauseThereIsNoMagicValue<openmode_flag>(filepath);

        // Gives the name vectors and pointer vectors a size if we're in read mode
        initMemberVectorsIfRequired<openmode_flag>();

        // Read the pointers that say where each string in the following section begins
        readWriter.template readWriteDataVector<uint32_t, LE>(bone_name_pointers);
        readWriter.template readWriteDataVector<uint32_t, LE>(material_name_pointers);

        // Read bone names
        for (uint32_t i = 0; i < bone_name_count - 1; i++)
        {
            readWriter.assertFilePointerIsAt(bone_name_pointers[i]);
            uint32_t string_length = bone_name_pointers[i + 1] - bone_name_pointers[i];
            readWriter.readWriteAscii(bone_names[i], string_length);
        }
        if (bone_name_count && material_name_count)
            readWriter.readWriteAscii(bone_names[bone_name_count - 1], material_name_pointers[0] - bone_name_pointers[bone_name_count - 1]);
        else
        {
            if (bone_name_count && !material_name_count)
                readWriter.readWriteAsciiToEOF(bone_names[bone_name_count - 1]);
        }

        // Read material names
        for (uint32_t i = 0; i < material_name_count - 1; i++)
        {
            readWriter.assertFilePointerIsAt(material_name_pointers[i]);
            uint32_t string_length = material_name_pointers[i + 1] - material_name_pointers[i];
            readWriter.readWriteAscii(material_names[i], string_length);
        }
        if (material_name_count)
            readWriter.readWriteAsciiToEOF(material_names[material_name_count - 1]);

        readWriter.assertAtEOF();
    }
}