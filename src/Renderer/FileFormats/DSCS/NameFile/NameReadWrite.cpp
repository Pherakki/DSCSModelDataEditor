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


    template<> 
    void NameReadWrite::checkThatThisMightActuallyBeANameFileBecauseThereIsNoMagicValue<readmode>(const std::string& filepath)
    {
        size_t filesize = std::filesystem::file_size(filepath);
        assert(filesize > (this->bone_name_count + this->material_name_count));
    }
    template<>
    void NameReadWrite::checkThatThisMightActuallyBeANameFileBecauseThereIsNoMagicValue<writemode>(const std::string & filepath)
    {}
}