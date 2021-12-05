#include "NameReadWrite.hpp"

namespace FileFormats::DSCS::NameFile
{
    // initMemberVectorsIfRequired
    template<>
    void NameReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        bone_name_pointers = std::vector<uint32_t>(bone_name_count);
        bone_names = std::vector<std::string>(bone_name_count);
        material_name_pointers = std::vector<uint32_t>(material_name_count);
        material_names = std::vector<std::string>(material_name_count);
    }
    template<>
    void NameReadWrite::initMemberVectorsIfRequired<writemode>() {}
}