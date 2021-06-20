#pragma once

#include <FileFormats/DSCS/GeomFile/ForwardDeclarations.hpp>
#include <serialisation/ReadWriter.hpp>

namespace FileFormats::DSCS::GeomFile
{
    enum class ShaderUniformType : uint8_t
    {
        DiffuseTextureID =     50,
        DiffuseColour =        51,
        NormalMapTextureID =   53,
        Bumpiness =            54,
        SpecularStrength =     56,
        SpecularPower =        57,
        CubeMapTextureID =     58,
        ReflectionStrength =   59,
        FresnelExp =           60,
        FresnelMin =           61,
        FuzzySpecColor =       62,
        SubColor =             63,
        SurfaceColor =         64,
        Rolloff =              65,
        VelvetStrength =       66,
        UnknownTextureSlot1 =  67,
        OverlayTextureID =     68,
        UnknownTextureSlot2 =  69,
        OverlayBumpiness =     70,
        OverlayStrength =      71,
        ToonTextureID =        72,
        Curvature =            75,
        GlassStrength =        76,
        UpsideDown =           77,
        ParallaxBiasX =        79,
        ParallaxBiasY =        80,
        Time =                 84,
        ScrollSpeedSet1 =      85,
        ScrollSpeedSet2 =      88,
        ScrollSpeedSet3 =      91,
        OffsetSet1 =           94,
        OffsetSet2 =           97,
        DistortionStrength =  100,
        LightMapStrength =    113,
        LightMapPower =       114,
        OffsetSet3 =          116,
        Fat =                 119,
        RotationSet1 =        120,
        RotationSet2 =        123,
        ScaleSet1 =           129,
        ZBias =               141,
        UnknownTextureSlot3 = 142
	};
	
	enum class OpenGLSettingType : uint8_t
	{
        Unknown0 = 160,
        Unknown1 = 161,
        Unknown2 = 162,
        Unknown3 = 163,
        Unknown4 = 164,
        Unknown5 = 166,
        Unknown6 = 167,
        Unknown7 = 168,
        Unknown8 = 169,
        Unknown9 = 172
	};

	struct ShaderUniform
	{
        std::array<uint8_t, 16> payload;       // 0x00 // Can be 1-4 floats, or 8 uint16s if set to 0 floats
        ShaderUniformType shader_uniform_type; // 0x10
        uint8_t num_floats_in_payload;         // 0x11 // States how many floats the payload contains
        uint16_t always_65280;                 // 0x12 // Always 65280
        uint32_t padding_0x14;                 // 0x14 // Always 0
	};

	struct OpenGLSetting
	{
        uint32_t payload_1;             // 0x00 // Can be two uint16s?
        uint32_t payload_2;             // 0x04 // Can be a float?
        uint32_t padding_0x08;          // 0x08 // Always 0
        uint32_t padding_0x0C;          // 0x0C // Always 0
        OpenGLSettingType setting_type; // 0x10
        uint8_t always_100;             // 0x11 // Always 100
        uint16_t always_65280;          // 0x12 // Always 65280
        uint32_t padding_0x14;          // 0x14 // Always 0
	};
	
	class MaterialReadWrite
	{
	private:
        // Header Variables
		uint32_t                name_hash = 0;                   // 0x00
		std::array<uint32_t, 4> shader_hash = {};                // 0x04
		uint8_t                 num_shader_uniforms = 0;         // 0x14
		uint8_t                 num_unknown_OpenGL_settings = 0; // 0x15
		uint16_t                enable_shadows = 0;              // 0x16

        // Data Holders
        std::vector<ShaderUniform> shader_uniforms = {};
        std::vector<OpenGLSetting> unknown_opengl_settings = {};

        // ReadWrite Methods

        template<std::ios_base::openmode openmode_flag> void readWriteHeader(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void initMemberVectorsIfRequired();
        template<std::ios_base::openmode openmode_flag> void readWriteShaderUniforms(ReadWriter<openmode_flag>&);
        template<std::ios_base::openmode openmode_flag> void readWriteOpenGLSettings(ReadWriter<openmode_flag>&);

        template<std::ios_base::openmode openmode_flag> void doReadWrite(ReadWriter<openmode_flag>&);

        // Friends
		friend class GeomReadWrite;
	};

    // **************************************** //
    // **************************************** //
    // *** PRIVATE FUNCTION IMPLEMENTATIONS *** //
    // **************************************** //
    // **************************************** //

    // readWriteHeader
    template<std::ios_base::openmode openmode_flag> 
    void MaterialReadWrite::readWriteHeader(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.ReadWriteData<uint32_t, LE>(name_hash);
        read_writer.ReadWriteData<std::array<uint32_t, 4>, LE>(shader_hash);
        read_writer.ReadWriteData<uint8_t, LE>(num_shader_uniforms);
        read_writer.ReadWriteData<uint8_t, LE>(num_unknown_OpenGL_settings);
        read_writer.ReadWriteData<uint16_t, LE>(enable_shadows);
    }

    // initMemberVectorsIfRequired
    template<> 
    void MaterialReadWrite::initMemberVectorsIfRequired<readmode>()
    {
        this->shader_uniforms = std::vector<ShaderUniform>(this->num_shader_uniforms);
        this->unknown_opengl_settings = std::vector<OpenGLSetting>(this->num_unknown_OpenGL_settings);
    }
    template<>
    void MaterialReadWrite::initMemberVectorsIfRequired<writemode>() {}

    // readWriteShaderUniforms
    template<std::ios_base::openmode openmode_flag>
    void MaterialReadWrite::readWriteShaderUniforms(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.ReadWriteDataArray<ShaderUniform, LE>(shader_uniforms);
    }

    // readWriteOpenGLSettings
    template<std::ios_base::openmode openmode_flag>
    void MaterialReadWrite::readWriteOpenGLSettings(ReadWriter<openmode_flag>& read_writer)
    {
        read_writer.ReadWriteDataArray<OpenGLSetting, LE>(unknown_opengl_settings);
    }

    template<std::ios_base::openmode openmode_flag> 
    void MaterialReadWrite::doReadWrite(ReadWriter<openmode_flag>& read_writer)
    {
        this->readWriteHeader<openmode_flag>(read_writer);
        this->initMemberVectorsIfRequired<openmode_flag>();
        this->readWriteShaderUniforms<openmode_flag>(read_writer);
        this->readWriteOpenGLSettings<openmode_flag>(read_writer);
    }
}