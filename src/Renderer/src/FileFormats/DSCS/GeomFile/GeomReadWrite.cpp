#include "GeomReadWrite.hpp"

namespace FileFormats::DSCS::GeomFile
{
    // initMemberVectorsIfRequired
    template<>
    void GeomReadWrite::initMemberVectorsIfRequired<readmode>()
    {

        this->meshes = std::vector<MeshReadWrite>(this->num_meshes);
        this->materials = std::vector<MaterialReadWrite>(this->num_materials);
        this->texture_names = std::vector<std::array<char, 32>>(this->getNumTextures());
        this->sec_4_data = std::vector<LightSourceDataStruct>(this->sec_4_count);
        this->sec_5_data = std::vector<CamDataStruct>(this->sec_5_count);
        this->inverse_bind_pose_matrices = std::vector<std::array<float, 12>>(this->num_inv_bind_pose_matrices);
        if (this->footer_data_section_ptr != 0)
            // This is a 1D texture, possibly unused. 16 byte header, 256 RGBA bytes, 12 pad bytes.
            this->unknown_footer_data = std::vector<uint8_t>(16 + (256 * 4) + 12);
    }
    template<>
    void GeomReadWrite::initMemberVectorsIfRequired<writemode>() {}

}