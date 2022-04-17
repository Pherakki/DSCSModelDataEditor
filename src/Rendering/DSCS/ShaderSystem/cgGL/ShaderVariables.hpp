#pragma once

#include <array>
#include <map>
#include <vector>

static const char* IN_vPOSITION = "Position";
static const char* IN_vNORMAL = "Normal";
static const char* IN_vTANGENT = "Tangent";
static const char* IN_vBINORMAL = "Binormal";
static const char* IN_vUV0 = "Texcoord0";
static const char* IN_vUV1 = "Texcoord1";
static const char* IN_vUV2 = "Texcoord2";
static const char* IN_vINDICES = "Indices";
static const char* IN_vWEIGHTS = "Weights";
static const std::vector<const char*> VERTEX_IN_PARAMS = { IN_vPOSITION, IN_vNORMAL, IN_vTANGENT, IN_vBINORMAL, IN_vUV0, IN_vUV1, IN_vUV2, IN_vINDICES, IN_vWEIGHTS };

static const char* IN_fPOSITION = "vPosition";
static const char* IN_fNORMAL = "vNormal";
static const char* IN_fTANGENT = "vTangent";
static const char* IN_fBINORMAL = "vBinormal";
static const char* IN_fUV0 = "vTexcoord0";
static const char* IN_fUV1 = "vTexcoord1";
static const char* IN_fUV2 = "vTexcoord2";
static const char* IN_fINDICES = "vIndices";
static const char* IN_fWEIGHTS = "vWeights";
static const std::vector<const char*> FRAGMENT_IN_PARAMS = { IN_fPOSITION, IN_fNORMAL, IN_fTANGENT, IN_fBINORMAL, IN_fUV0, IN_fUV1, IN_fUV2, IN_fINDICES, IN_fWEIGHTS };

static const enum class ShaderType : uint8_t
{
    FS,
    VS
};

static const enum class ShaderUniformType : uint8_t
{
    Texture,
    Vector,
    Matrix,
    Array
};

static const struct ShaderUniformProperties
{
	std::string name;
	uint8_t id;
    ShaderType shader_type;
    ShaderUniformType shader_uniform_type;
	uint16_t elements;
};

static const std::array<ShaderUniformProperties, 0xA0> shader_uniform_properties =
{ 
    {
        { "Unknown",                0x00, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "ViewProj",               0x01, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "World",                  0x02, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "View",                   0x03, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "ViewInverse",            0x04, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "WorldInverse",           0x05, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "MatrixPalette",          0x06, ShaderType::VS, ShaderUniformType::Array,   672 }, // Unused
        { "SystemAlpha",            0x07, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "ViewportPixelSize",      0x08, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "DirLamp01Color",         0x09, ShaderType::VS, ShaderUniformType::Vector,    4 }, // Unused
        { "ColorSamplerScale",      0x0a, ShaderType::FS, ShaderUniformType::Vector,    2 }, // Unused
        { "DirLamp01Dir",           0x0b, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "DirLamp02Color",         0x0c, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "bloomLimit",             0x0d, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "DirLamp02Dir",           0x0e, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "DirLamp03Color",         0x0f, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "blurOffset",             0x10, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "DirLamp03Dir",           0x11, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "PointLamp01Color",       0x12, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "ShadowDensity",          0x13, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp01Range",       0x14, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp01Pos",         0x15, ShaderType::VS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp02Color",       0x16, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "bloomScale",             0x17, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp02Range",       0x18, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp02Pos",         0x19, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp03Color",       0x1a, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "ColorCorrectionSampler", 0x1b, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "PointLamp03Range",       0x1c, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp03Pos",         0x1d, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp04Color",       0x1e, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "BlurredSampler",         0x1f, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "PointLamp04Range",       0x20, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp04Pos",         0x21, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp05Color",       0x22, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "DepthSampler",           0x23, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "PointLamp05Range",       0x24, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp05Pos",         0x25, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp06Color",       0x26, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "dofParams",              0x27, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp06Range",       0x28, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp06Pos",         0x29, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "PointLamp07Color",       0x2a, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "dofOptions",             0x2b, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "PointLamp07Range",       0x2c, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "PointLamp07Pos",         0x2d, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "AmbientColor",           0x2e, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "SkyDir",                 0x2f, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "dofGrayScale",           0x30, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "GroundColor",            0x31, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "ColorSampler",           0x32, ShaderType::FS, ShaderUniformType::Texture,   0 }, // DiffuseTextureID
        { "DiffuseColor",           0x33, ShaderType::FS, ShaderUniformType::Vector,    4 },
        { "DiffuseAlpha",           0x34, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "NormalSampler",          0x35, ShaderType::FS, ShaderUniformType::Texture,   0 }, // NormalMapTextureID
        { "Bumpiness",              0x36, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "SpecularParams",         0x37, ShaderType::FS, ShaderUniformType::Vector,    2 },
        { "SpecularStrength",       0x38, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "SpecularPower",          0x39, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "EnvSampler",             0x3a, ShaderType::FS, ShaderUniformType::Texture,   0 }, // CubeMapTextureID
        { "ReflectionStrength",     0x3b, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "FresnelMin",             0x3c, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "FresnelExp",             0x3d, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "SurfaceColor",           0x3e, ShaderType::FS, ShaderUniformType::Vector,    3 },
        { "FuzzySpecColor",         0x3f, ShaderType::FS, ShaderUniformType::Vector,    3 },
        { "SubColor",               0x40, ShaderType::FS, ShaderUniformType::Vector,    3 },
        { "RollOff",                0x41, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "VelvetStrength",         0x42, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "LightSampler",           0x43, ShaderType::FS, ShaderUniformType::Texture,   0 }, // UnknownTextureSlot1
        { "OverlayColorSampler",    0x44, ShaderType::FS, ShaderUniformType::Texture,   0 }, // OverlayTextureID
        { "OverlayNormalSampler",   0x45, ShaderType::FS, ShaderUniformType::Texture,   0 }, // UnknownTextureSlot2
        { "OverlayBumpiness",       0x46, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "OverlayStrength",        0x47, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "CLUTSampler",            0x48, ShaderType::FS, ShaderUniformType::Texture,   0 }, // ToonTexture
        { "BackBufferSampler",      0x49, ShaderType::FS, ShaderUniformType::Texture,   0 },
        { "GlassParams",            0x4a, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "GlassStrength",          0x4b, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "Curvature",              0x4c, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "UpsideDown",             0x4d, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "ParallaxBias",           0x4e, ShaderType::FS, ShaderUniformType::Vector,    2 }, // Unused
        { "ParallaxBiasX",          0x4f, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "ParallaxBiasY",          0x50, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "ShadowSampler",          0x51, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "LightViewProj",          0x52, ShaderType::VS, ShaderUniformType::Matrix,   16 }, // Unused
        { "CameraPosition",         0x53, ShaderType::VS, ShaderUniformType::Vector,    3 }, // Unused
        { "Time",                   0x54, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "ScrollSpeedSet1",        0x55, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "LightColor",             0x56, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "MulColor",               0x57, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "ScrollSpeedSet2",        0x58, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "ScrollSpeedSet2U",       0x59, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScrollSpeedSet2V",       0x5a, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScrollSpeedSet3",        0x5b, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "ScrollSpeedSet3U",       0x5c, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScrollSpeedSet3V",       0x5d, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "OffsetSet1",             0x5e, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet1U",            0x5f, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet1V",            0x60, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet2",             0x61, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet2U",            0x62, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet2V",            0x63, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "DistortionStrength",     0x64, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "FogParams",              0x65, ShaderType::VS, ShaderUniformType::Vector,    3 }, // Unused
        { "FogNear",                0x66, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "FogFar",                 0x67, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "FogAlpha",               0x68, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "FogColor",               0x69, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "AlphaRef",               0x6a, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "CoverSampler",           0x6b, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "CoverScale",             0x6c, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "CoverScaleH",            0x6d, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "CoverScaleV",            0x6e, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "CoverScaleAddition",     0x6f, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "MipBias",                0x70, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "LightMapPower",          0x71, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "LightMapStrength",       0x72, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "Saturation",             0x73, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "OffsetSet3",             0x74, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "OffsetSet3U",            0x75, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "OffsetSet3V",            0x76, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "Fat",                    0x77, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "RotationSet1",           0x78, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "dumy121",                0x79, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "dumy122",                0x7a, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "RotationSet2",           0x7b, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "dumy124",                0x7c, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "dumy125",                0x7d, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "RotationSet3",           0x7e, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "dumy127",                0x7f, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "dumy128",                0x80, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet1",              0x81, ShaderType::VS, ShaderUniformType::Vector,    2 },
        { "ScaleSet1U",             0x82, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet1V",             0x83, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet2",              0x84, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "ScaleSet2U",             0x85, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet2V",             0x86, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet3",              0x87, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "ScaleSet3U",             0x88, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ScaleSet3V",             0x89, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "CoverOffset",            0x8a, ShaderType::VS, ShaderUniformType::Vector,    2 }, // Unused
        { "CoverOffsetX",           0x8b, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "CoverOffsetY",           0x8c, ShaderType::VS, ShaderUniformType::Vector,    1 }, // Unused
        { "ZBias",                  0x8d, ShaderType::VS, ShaderUniformType::Vector,    1 },
        { "EnvsSampler",            0x8e, ShaderType::FS, ShaderUniformType::Texture,   0 }, // UnknownTextureSlot3
        { "InnerGrowAValue",        0x8f, ShaderType::FS, ShaderUniformType::Vector,    3 },
        { "InnerGrowAPower",        0x90, ShaderType::FS, ShaderUniformType::Vector,    1 },
        { "InnerGrowAStrength",     0x91, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "InnerGrowALimit",        0x92, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "GlowACLUTSampler",       0x93, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "InnerGrowBValue",        0x94, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "InnerGrowBPower",        0x95, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "InnerGrowBStrength",     0x96, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "InnerGrowBLimit",        0x97, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "GlowBCLUTSampler",       0x98, ShaderType::FS, ShaderUniformType::Texture,   0 }, // Unused
        { "LocalIntensity",         0x99, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "InnerGrowAColor",        0x9a, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "InnerGrowBColor",        0x9b, ShaderType::FS, ShaderUniformType::Vector,    3 }, // Unused
        { "SepiaStrength",          0x9c, ShaderType::FS, ShaderUniformType::Vector,    1 }, // Unused
        { "ViewOffset",             0x9d, ShaderType::VS, ShaderUniformType::Vector,    3 }, // Unused
        { "ScreenVertexSize",       0x9e, ShaderType::FS, ShaderUniformType::Vector,    4 }, // Unused
        { "ColorTextureSize",       0x9f, ShaderType::FS, ShaderUniformType::Vector,    4 }  // Unused
    }
};

// Surely this can be compile-time initialised from the preceeding array?!
static const std::map<std::string, uint8_t> uniform_name_to_id =
{
    { "Unknown",                0x00 },
    { "ViewProj",               0x01 },
    { "World",                  0x02 },
    { "View",                   0x03 },
    { "ViewInverse",            0x04 },
    { "WorldInverse",           0x05 },
    { "MatrixPalette",          0x06 },
    { "SystemAlpha",            0x07 },
    { "ViewportPixelSize",      0x08 },
    { "DirLamp01Color",         0x09 },
    { "ColorSamplerScale",      0x0a },
    { "DirLamp01Dir",           0x0b },
    { "DirLamp02Color",         0x0c },
    { "bloomLimit",             0x0d },
    { "DirLamp02Dir",           0x0e },
    { "DirLamp03Color",         0x0f },
    { "blurOffset",             0x10 },
    { "DirLamp03Dir",           0x11 },
    { "PointLamp01Color",       0x12 },
    { "ShadowDensity",          0x13 },
    { "PointLamp01Range",       0x14 },
    { "PointLamp01Pos",         0x15 },
    { "PointLamp02Color",       0x16 },
    { "bloomScale",             0x17 },
    { "PointLamp02Range",       0x18 },
    { "PointLamp02Pos",         0x19 },
    { "PointLamp03Color",       0x1a },
    { "ColorCorrectionSampler", 0x1b },
    { "PointLamp03Range",       0x1c },
    { "PointLamp03Pos",         0x1d },
    { "PointLamp04Color",       0x1e },
    { "BlurredSampler",         0x1f },
    { "PointLamp04Range",       0x20 },
    { "PointLamp04Pos",         0x21 },
    { "PointLamp05Color",       0x22 },
    { "DepthSampler",           0x23 },
    { "PointLamp05Range",       0x24 },
    { "PointLamp05Pos",         0x25 },
    { "PointLamp06Color",       0x26 },
    { "dofParams",              0x27 },
    { "PointLamp06Range",       0x28 },
    { "PointLamp06Pos",         0x29 },
    { "PointLamp07Color",       0x2a },
    { "dofOptions",             0x2b },
    { "PointLamp07Range",       0x2c },
    { "PointLamp07Pos",         0x2d },
    { "AmbientColor",           0x2e },
    { "SkyDir",                 0x2f },
    { "dofGrayScale",           0x30 },
    { "GroundColor",            0x31 },
    { "ColorSampler",           0x32 },
    { "DiffuseColor",           0x33 },
    { "DiffuseAlpha",           0x34 },
    { "NormalSampler",          0x35 },
    { "Bumpiness",              0x36 },
    { "SpecularParams",         0x37 },
    { "SpecularStrength",       0x38 },
    { "SpecularPower",          0x39 },
    { "EnvSampler",             0x3a },
    { "ReflectionStrength",     0x3b },
    { "FresnelMin",             0x3c },
    { "FresnelExp",             0x3d },
    { "SurfaceColor",           0x3e },
    { "FuzzySpecColor",         0x3f },
    { "SubColor",               0x40 },
    { "RollOff",                0x41 },
    { "VelvetStrength",         0x42 },
    { "LightSampler",           0x43 },
    { "OverlayColorSampler",    0x44 },
    { "OverlayNormalSampler",   0x45 },
    { "OverlayBumpiness",       0x46 },
    { "OverlayStrength",        0x47 },
    { "CLUTSampler",            0x48 },
    { "BackBufferSampler",      0x49 },
    { "GlassParams",            0x4a },
    { "GlassStrength",          0x4b },
    { "Curvature",              0x4c },
    { "UpsideDown",             0x4d },
    { "ParallaxBias",           0x4e },
    { "ParallaxBiasX",          0x4f },
    { "ParallaxBiasY",          0x50 },
    { "ShadowSampler",          0x51 },
    { "LightViewProj",          0x52 },
    { "CameraPosition",         0x53 },
    { "Time",                   0x54 },
    { "ScrollSpeedSet1",        0x55 },
    { "LightColor",             0x56 },
    { "MulColor",               0x57 },
    { "ScrollSpeedSet2",        0x58 },
    { "ScrollSpeedSet2U",       0x59 },
    { "ScrollSpeedSet2V",       0x5a },
    { "ScrollSpeedSet3",        0x5b },
    { "ScrollSpeedSet3U",       0x5c },
    { "ScrollSpeedSet3V",       0x5d },
    { "OffsetSet1",             0x5e },
    { "OffsetSet1U",            0x5f },
    { "OffsetSet1V",            0x60 },
    { "OffsetSet2",             0x61 },
    { "OffsetSet2U",            0x62 },
    { "OffsetSet2V",            0x63 },
    { "DistortionStrength",     0x64 },
    { "FogParams",              0x65 },
    { "FogNear",                0x66 },
    { "FogFar",                 0x67 },
    { "FogAlpha",               0x68 },
    { "FogColor",               0x69 },
    { "AlphaRef",               0x6a },
    { "CoverSampler",           0x6b },
    { "CoverScale",             0x6c },
    { "CoverScaleH",            0x6d },
    { "CoverScaleV",            0x6e },
    { "CoverScaleAddition",     0x6f },
    { "MipBias",                0x70 },
    { "LightMapPower",          0x71 },
    { "LightMapStrength",       0x72 },
    { "Saturation",             0x73 },
    { "OffsetSet3",             0x74 },
    { "OffsetSet3U",            0x75 },
    { "OffsetSet3V",            0x76 },
    { "Fat",                    0x77 },
    { "RotationSet1",           0x78 },
    { "dumy121",                0x79 },
    { "dumy122",                0x7a },
    { "RotationSet2",           0x7b },
    { "dumy124",                0x7c },
    { "dumy125",                0x7d },
    { "RotationSet3",           0x7e },
    { "dumy127",                0x7f },
    { "dumy128",                0x80 },
    { "ScaleSet1",              0x81 },
    { "ScaleSet1U",             0x82 },
    { "ScaleSet1V",             0x83 },
    { "ScaleSet2",              0x84 },
    { "ScaleSet2U",             0x85 },
    { "ScaleSet2V",             0x86 },
    { "ScaleSet3",              0x87 },
    { "ScaleSet3U",             0x88 },
    { "ScaleSet3V",             0x89 },
    { "CoverOffset",            0x8a },
    { "CoverOffsetX",           0x8b },
    { "CoverOffsetY",           0x8c },
    { "ZBias",                  0x8d },
    { "EnvsSampler",            0x8e },
    { "InnerGrowAValue",        0x8f },
    { "InnerGrowAPower",        0x90 },
    { "InnerGrowAStrength",     0x91 },
    { "InnerGrowALimit",        0x92 },
    { "GlowACLUTSampler",       0x93 },
    { "InnerGrowBValue",        0x94 },
    { "InnerGrowBPower",        0x95 },
    { "InnerGrowBStrength",     0x96 },
    { "InnerGrowBLimit",        0x97 },
    { "GlowBCLUTSampler",       0x98 },
    { "LocalIntensity",         0x99 },
    { "InnerGrowAColor",        0x9a },
    { "InnerGrowBColor",        0x9b },
    { "SepiaStrength",          0x9c },
    { "ViewOffset",             0x9d },
    { "ScreenVertexSize",       0x9e },
    { "ColorTextureSize",       0x9f },
};

static const auto world_uniform_ids =
std::array{
    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0a,
    0x0b,
    0x0c,
    0x0d,
    0x0e,
    0x0f,
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1a,

    0x1c,
    0x1d,
    0x1e,

    0x20,
    0x21,
    0x22,

    0x24,
    0x25,
    0x26,
    0x27,
    0x28,
    0x29,
    0x2a,
    0x2b,
    0x2c,
    0x2d,
    0x2e,
    0x2f,
    0x30,
    0x31,

    0x52,
    0x53,
    0x54,

    0x65,
    0x66,
    0x67,
    0x68,
    0x69,

    0x9d,
    0x9e,
    0x9f
};

static const auto texture_ids =
std::array
{
    0x1b, 
      
    0x1f, 
      
    0x23, 
      
    0x32, 
      
    0x35, 
      
    0x3a, 
      
    0x43, 
    0x44, 
    0x45, 
      
    0x48, 
    0x49, 
      
    0x51, 
      
    0x6b, 
      
    0x8e, 
      
    0x93, 
      
    0x98
};