#pragma once

#include "GeomForwardDeclarations.hpp"
#include "serialisation/ReadWriter.hpp"

namespace FileFormats::DSCS::GeomFile
{
	struct ShaderUniform
	{
        std::array<float, 4> payload;          // 0x00 // Can be 1-4 floats, or 8 uint16s if set to 0 floats
        uint8_t shader_uniform_type;           // 0x10
        uint8_t num_floats_in_payload;         // 0x11 // States how many floats the payload contains
        uint16_t always_65280;                 // 0x12 // Always 65280
        uint32_t padding_0x14;                 // 0x14 // Always 0
	};

	struct OpenGLSetting
	{
        std::array<uint32_t, 4> payload; // 0x00
        uint8_t setting_type;            // 0x10
        uint8_t always_100;              // 0x11 // Always 100
        uint16_t always_65280;           // 0x12 // Always 65280
        uint32_t padding_0x14;           // 0x14 // Always 0
	};
	
	class MaterialReadWrite
	{
	public:
        // Header Variables
		uint32_t                name_hash = 0;                   // 0x00
		std::array<uint32_t, 4> shader_hash = {};                // 0x04
		uint8_t                 num_shader_uniforms = 0;         // 0x14
		uint8_t                 num_OpenGL_settings = 0;         // 0x15
		uint16_t                enable_shadows = 0;              // 0x16

        // Data Holders
        std::vector<ShaderUniform> shader_uniforms = {};
        std::vector<OpenGLSetting> opengl_settings = {};

        // ReadWrite Methods

        template<std::ios_base::openmode openmode_flag> void readWriteHeader(serialisation::ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteShaderUniforms(serialisation::ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteOpenGLSettings(serialisation::ReadWriter<openmode_flag>&);

        template<std::ios_base::openmode openmode_flag> void doReadWrite(serialisation::ReadWriter<openmode_flag>&);

        // Friends
		friend class GeomReadWrite;
        friend class DSCStoOpenGL;
	};

    // **************************************** //
    // **************************************** //
    // *** PRIVATE FUNCTION IMPLEMENTATIONS *** //
    // **************************************** //
    // **************************************** //

    // readWriteHeader
    template<std::ios_base::openmode openmode_flag> 
    void MaterialReadWrite::readWriteHeader(serialisation::ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.template readWriteData<uint32_t, serialisation::LE>(name_hash);
        read_writer.template readWriteData<std::array<uint32_t, 4>, serialisation::LE>(shader_hash);
        read_writer.template readWriteData<uint8_t, serialisation::LE>(num_shader_uniforms);
        read_writer.template readWriteData<uint8_t, serialisation::LE>(num_OpenGL_settings);
        read_writer.template readWriteData<uint16_t, serialisation::LE>(enable_shadows);
    }


    // readWriteShaderUniforms
    template<std::ios_base::openmode openmode_flag>
    void MaterialReadWrite::readWriteShaderUniforms(serialisation::ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.template readWriteDataVector<ShaderUniform, serialisation::LE>(this->shader_uniforms);
    }

    // readWriteOpenGLSettings
    template<std::ios_base::openmode openmode_flag>
    void MaterialReadWrite::readWriteOpenGLSettings(serialisation::ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.template readWriteDataVector<OpenGLSetting, serialisation::LE>(this->opengl_settings);
    }

    template<std::ios_base::openmode openmode_flag> 
    void MaterialReadWrite::doReadWrite(serialisation::ReadWriter<openmode_flag>& read_writer)
    {
        this->readWriteHeader<openmode_flag>(read_writer);
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteShaderUniforms<openmode_flag>(read_writer);
        this->readWriteOpenGLSettings<openmode_flag>(read_writer);
    }
}