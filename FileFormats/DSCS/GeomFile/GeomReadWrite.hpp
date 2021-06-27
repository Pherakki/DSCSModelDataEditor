#pragma once

#include <array>

#include <serialisation/Exceptions.hpp>
#include <serialisation/ReadWriter.hpp>
#include <serialisation/Macros.hpp>

#include <FileFormats/DSCS/DSCSModelForwardDeclaration.hpp>
#include <FileFormats/DSCS/GeomFile/MeshReadWrite.hpp>
#include <FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp>
#include <FileFormats/DSCS/GeomFile/Section4ReadWrite.hpp>
#include <FileFormats/DSCS/GeomFile/Section5ReadWrite.hpp>

using namespace serialisation;

namespace FileFormats::DSCS::GeomFile
{
	class GeomReadWrite
	{
    public:
        GeomReadWrite() {};
        ~GeomReadWrite() {};
        void read(const std::string& filepath) { doReadWrite<readmode>(filepath); };
        void write(const std::string& filepath) { doReadWrite<writemode>(filepath); };

        std::vector<MeshReadWrite> getMeshes();
        uint32_t getNumTextures() { return (uint32_t)(this->texture_names_section_bytecount / 32); };

        // Friends
        friend class DSCStoOpenGL;

        // Header Variables
        const uint32_t       magic_value = 100;                          // d000  // 0x00
        uint16_t             num_meshes = 0;                                      // 0x04
        uint16_t             num_materials = 0;                                   // 0x06
        uint16_t             sec_4_count = 0;                                     // 0x08
        uint16_t             sec_5_count = 0;                                     // 0x0A
        uint32_t             num_inv_bind_pose_matrices = 0;                      // 0x0C
        uint32_t             texture_names_section_bytecount = 0;                 // 0x10
        std::array<float, 3> geom_centre = { 0.f, 0.f, 0.f};                      // 0x14
        std::array<float, 3> geom_bounding_box_halflengths = { 0.f, 0.f, 0.f };   // 0x20
        const uint32_t       padding_0x2C = 0;                                    // 0x2C
        uint64_t             meshes_section_ptr = 0;                              // 0x30
        uint64_t             materials_section_ptr = 0;                           // 0x38
        uint64_t             sec_4_ptr = 0;                                       // 0x40
        uint64_t             sec_5_ptr = 0;                                       // 0x48
        uint64_t             inv_bind_pose_matrices_section_ptr = 0;              // 0x50
        const uint64_t       padding_0x58 = 0;                                    // 0x58
        uint64_t             texture_names_section_ptr = 0;                       // 0x60
        uint64_t             footer_data_section_ptr = 0;                         // 0x68

        // Data Holders
        std::vector<MeshReadWrite> meshes = {};
        std::vector<MaterialReadWrite> materials = {};
        std::vector<std::array<char, 32>> texture_names = {};
        std::vector<Sec4ReadWrite> sec_4_data = {};
        std::vector<Sec5ReadWrite> sec_5_data = {};
        std::vector<std::array<float, 12>> inverse_bind_pose_matrices = {};
        std::vector<uint8_t> unknown_footer_data = {};

        // Helper functions
        template<typename T> bool sectionIsNDef(T ptr_value, std::string ptr_name, T count_value, std::string count_name);

        // RW Methods
        template<std::ios_base::openmode openmode_flag> void readWriteHeader(ReadWriter<openmode_flag> &);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteMeshes(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteMaterials(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteTextureNames(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteSection4Data(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteSection5Data(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteInverseBindPoseMatrices(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteFooterData(ReadWriter<openmode_flag>&);

        template<std::ios_base::openmode openmode_flag> void doReadWrite(const std::string&);


	};

    // **************************************** //
    // **************************************** //
    // *** PRIVATE FUNCTION IMPLEMENTATIONS *** //
    // **************************************** //
    // **************************************** //
 
    // ******************************** //
    // Helper Function Implementations //
    // ******************************** //
    template<typename T>
    bool GeomReadWrite::sectionIsNDef(T ptr_value, std::string ptr_name, T count_value, std::string count_name)
    {
        if (ptr_value == 0)
        {
            if (ptr_value != count_value)
                throw InconsistentFilePointerAndCountException<T>(ptr_name, ptr_value, count_name, count_value);
            return true;
        }
        return false;
    }

    // ********************************** //
    // ReadWrite Function Implementations //
    // ********************************** //

    // readWriteHeader
    template<std::ios_base::openmode openmode_flag> 
    void GeomReadWrite::readWriteHeader(ReadWriter<openmode_flag> &read_writer)
    {
        read_writer.ReadWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(magic_value));
        // Lots of section size counts
        read_writer.ReadWriteData<uint16_t, LE>(this->num_meshes);
        read_writer.ReadWriteData<uint16_t, LE>(this->num_materials);
        read_writer.ReadWriteData<uint16_t, LE>(this->sec_4_count);
        read_writer.ReadWriteData<uint16_t, LE>(this->sec_5_count);
        read_writer.ReadWriteData<uint32_t, LE>(this->num_inv_bind_pose_matrices);
        read_writer.ReadWriteData<uint32_t, LE>(this->texture_names_section_bytecount);

        // Bounding Box
        read_writer.ReadWriteData<std::array<float, 3>, LE>(this->geom_centre);
        read_writer.ReadWriteData<std::array<float, 3>, LE>(this->geom_bounding_box_halflengths);
        read_writer.ReadWriteConstData<uint32_t, LE>(this->VALUE_AND_NAME_OF(padding_0x2C));

        // A bunch of pointers to various sections
        read_writer.ReadWriteData<uint64_t, LE>(this->meshes_section_ptr);
        read_writer.ReadWriteData<uint64_t, LE>(this->materials_section_ptr);
        read_writer.ReadWriteData<uint64_t, LE>(this->sec_4_ptr);
        read_writer.ReadWriteData<uint64_t, LE>(this->sec_5_ptr);
        read_writer.ReadWriteData<uint64_t, LE>(this->inv_bind_pose_matrices_section_ptr);
        read_writer.ReadWriteConstData<uint64_t, LE>(this->VALUE_AND_NAME_OF(padding_0x58));
        read_writer.ReadWriteData<uint64_t, LE>(this->texture_names_section_ptr);
        read_writer.ReadWriteData<uint64_t, LE>(this->footer_data_section_ptr);
    }

    // initMemberVectorsIfRequired
    template<>
    void GeomReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        
        this->meshes =                     std::vector<MeshReadWrite>(this->num_meshes);
        this->materials =                  std::vector<MaterialReadWrite>(this->num_materials);
        this->texture_names =              std::vector<std::array<char, 32>>(this->getNumTextures());      
        this->sec_4_data =                 std::vector<Sec4ReadWrite>(this->sec_4_count);
        this->sec_5_data =                 std::vector<Sec5ReadWrite>(this->sec_5_count);
        this->inverse_bind_pose_matrices = std::vector<std::array<float, 12>>(this->num_inv_bind_pose_matrices);
        if (this->footer_data_section_ptr != 0)
            // 16 bytes that might be useful, each byte value repeated 4 times with some variation in value, and 12 padding bytes.
            this->unknown_footer_data =    std::vector<uint8_t>(16 + (256 * 4) + 12);
    }
    template<>
    void GeomReadWrite::initMemberVectorsIfRequired<writemode>() {}


    // readWriteMeshes
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteMeshes(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint32_t>(this->VALUE_AND_NAME_OF(meshes_section_ptr), 
                                          this->VALUE_AND_NAME_OF(num_meshes)))
            return;
        read_writer.AssertFilePointerIsAt(this->meshes_section_ptr);
        for (uint16_t mesh_i = 0; mesh_i < this->num_meshes; mesh_i++)
            this->meshes[mesh_i].readWriteHeader<openmode_flag>(read_writer);
        for (uint16_t mesh_i = 0; mesh_i < this->num_meshes; mesh_i++)
            this->meshes[mesh_i].readWriteBody<openmode_flag>(read_writer);
    }

    // readWriteMaterials
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteMaterials(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint64_t>(this->VALUE_AND_NAME_OF(materials_section_ptr), 
                                          this->VALUE_AND_NAME_OF(num_materials)))
            return;
        read_writer.AssertFilePointerIsAt(this->materials_section_ptr);
        for (uint16_t mat_i = 0; mat_i < this->num_materials; mat_i++)
            this->materials[mat_i].doReadWrite<openmode_flag>(read_writer);
    }

    // readWriteTextureNames
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteTextureNames(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint64_t>(this->VALUE_AND_NAME_OF(texture_names_section_ptr),
                                          this->VALUE_AND_NAME_OF(getNumTextures())))
            return; 
        read_writer.AssertFilePointerIsAt(this->texture_names_section_ptr);
        read_writer.ReadWriteDataArray<std::array<char, 32>, LE>(this->texture_names);
    }

    // readWriteSection4Data
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteSection4Data(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint64_t>(this->VALUE_AND_NAME_OF(sec_4_ptr),
                                          this->VALUE_AND_NAME_OF(sec_4_count)))
            return;
        read_writer.AssertFilePointerIsAt(this->sec_4_ptr);
        read_writer.ReadWriteDataArray<Sec4ReadWrite, LE>(this->sec_4_data);
    }

    // readWriteSection5Data
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteSection5Data(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint64_t>(this->VALUE_AND_NAME_OF(sec_5_ptr),
                                          this->VALUE_AND_NAME_OF(sec_5_count)))
            return; 
        read_writer.AssertFilePointerIsAt(this->sec_5_ptr);
        read_writer.ReadWriteDataArray<Sec5ReadWrite, LE>(this->sec_5_data);
    }

    // readWriteInverseBindPoseMatrices
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteInverseBindPoseMatrices(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->sectionIsNDef<uint64_t>(this->VALUE_AND_NAME_OF(inv_bind_pose_matrices_section_ptr),
                                          this->VALUE_AND_NAME_OF(num_inv_bind_pose_matrices)))
            return;
        read_writer.AssertFilePointerIsAt(this->inv_bind_pose_matrices_section_ptr);
        read_writer.ReadWriteDataArray<std::array<float, 12>, LE>(this->inverse_bind_pose_matrices);
    }

    // readWriteFooterData
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::readWriteFooterData(ReadWriter<openmode_flag>& read_writer)
    {
        if (this->footer_data_section_ptr == 0)
            return;
        read_writer.AssertFilePointerIsAt(this->footer_data_section_ptr);
        read_writer.ReadWriteDataArray<uint8_t, LE>(this->unknown_footer_data);
    }

    // doReadWrite
    template<std::ios_base::openmode openmode_flag>
    void GeomReadWrite::doReadWrite(const std::string& filepath)
    {
        ReadWriter<openmode_flag> read_writer(filepath);

        this->readWriteHeader<openmode_flag>(read_writer);
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteMeshes<openmode_flag>(read_writer);
        this->readWriteMaterials<openmode_flag>(read_writer);
        this->readWriteTextureNames<openmode_flag>(read_writer);
        this->readWriteSection4Data<openmode_flag>(read_writer);
        this->readWriteSection5Data<openmode_flag>(read_writer);
        read_writer.ReadWriteBytesLeftInIncompleteChunk(read_writer.Tell(), 16);
        this->readWriteInverseBindPoseMatrices<openmode_flag>(read_writer);
        this->readWriteFooterData<openmode_flag>(read_writer);

        read_writer.AssertAtEOF();
    }

    // *************************************** //
    // *************************************** //
    // *** PUBLIC FUNCTION IMPLEMENTATIONS *** //
    // *************************************** //
    // *************************************** //
}