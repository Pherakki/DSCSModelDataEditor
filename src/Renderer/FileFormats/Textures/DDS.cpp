#include <fstream>

#include "../../glad/include/glad/glad.h"
#include "DDS.hpp"
#include "../../serialisation/ReadWriter.hpp"
#include "../../Cg/cg.h"
#include "../../Cg/cgGL.h"
#include "../../Utils/OpenGL.hpp"


GLuint loadDDS(const char* path, TextureType tex_type)
{
    FILE* f;
    f = fopen(path, "rb");
    if (f == 0)
        return 0;

    // make sure this is a dds
    char filecode[4];
    fread(&filecode, 1, 4, f);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(f);
        // Replace with something catchable
        throw std::runtime_error("Not a DDS!");
    }

    unsigned char header[124];
    fread(&header, 1, 124, f);

    // different depending on byte arrangement/endianess, this is the windows version
    unsigned int width = (header[8]) | (header[9] << 8) | (header[10] << 16) | (header[11] << 24);
    unsigned int height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
    unsigned int linearSize = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
    unsigned int mipMapCount = (header[24]) | (header[25] << 8) | (header[26] << 16) | (header[27] << 24);

    // a 4-byte array of chars
    char* fourCC = (char*)&(header[80]);

    // decide byte format and block size from type
    // probably should check if the EXT is available
    unsigned int blockSize;
    unsigned int format;
    bool compressed = true;
    if (strncmp(fourCC, "DXT1", 4) == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        blockSize = 8;
    }
    else if (strncmp(fourCC, "DXT3", 4) == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        blockSize = 16;
    }
    else if (strncmp(fourCC, "DXT5", 4) == 0) {
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        blockSize = 16;
    }
    else if (strncmp(fourCC, "", 4) == 0)
    {
        format = 0;
        blockSize = 0;
        compressed = false;
    }
    else {
        // unhandled type
        // Replace with something catchable
        std::string debug_string = "Unknown compression type \'" + std::string(fourCC) + "\'";
        throw std::runtime_error(debug_string.c_str());
    }

    // 'the sum of all the mipmap's byte-size' is never greater than 'two times the biggest mipmap byte-size'
    if (!compressed)
        linearSize = 4 * width * height;
    unsigned int buffer_size = (mipMapCount > 1 ? linearSize + linearSize : linearSize);
    unsigned char* buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));
    if (buffer == 0) {
        // Handle OOM
        // Replace with something catchable
        throw std::runtime_error("BUFFER HAS 0 SIZE");
    }

    fread(buffer, 1, buffer_size, f);

    fclose(f);

    // glGenTextures() CAN return 0 on error, see docs - probably won't happen unless you are bad
    GLuint tid;
    glGenTextures(1, &tid);
    if (tid == 0) {
        free(buffer);
        // Replace with something catchable
        throw std::runtime_error("GEN TEXTURES RETURNED 0");
    }
    checkGLError();

    uint16_t texture_slot;
    switch (tex_type)
    {
    case TextureType::Texture2D:
    case TextureType::TextureLuminance:
        texture_slot = GL_TEXTURE_2D;

        glBindTexture(texture_slot, tid);
        // dds gives you the wrong amount of mipMaps, MAX_LEVEL 0 = 1 mipmap, MAX_LEVEL 1 = 2 mipmaps
        // thusly just minus one - simple off-by-one array fix
        glTexParameteri(texture_slot, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(texture_slot, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
        // Correct for a "standard" texture
        glTexParameteri(texture_slot, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_slot, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
        // These should be set on a per-texture basis
    case TextureType::TextureCLUT:
        texture_slot = GL_TEXTURE_2D;

        glBindTexture(texture_slot, tid);
        glTexParameteri(texture_slot, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(texture_slot, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
        glTexParameteri(texture_slot, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(texture_slot, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case TextureType::TextureCube:
        texture_slot = GL_TEXTURE_CUBE_MAP;

        // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(texture_slot, tid);
        glTexParameteri(texture_slot, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(texture_slot, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
        glTexParameteri(texture_slot, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(texture_slot, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture_slot, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;
    default:
        throw std::runtime_error("Unknown Texture Type");
    }
    checkGLError();




    unsigned int offset = 0;
    for (unsigned int level = 0; level < mipMapCount && (width || height); level++) {
        // ignore mipmaps you will never see, save 4*4*4 + 2*2*4 + 1*1*4 data I guess
        // if you don't at least ignore the 0*0 mipmaps, you will 0/2
        // dds can have mipmaps that aren't rectangular, thusly 256*128 and 128*64 and 1*0 can be levels
        if (width <= 4 || height <= 4) {
            // -1 because this iteration of level is the next, which we don't use
            glTexParameteri(texture_slot, GL_TEXTURE_MAX_LEVEL, level - 1);
            break;
        }

        unsigned int size;
        if (compressed)
        {
            // the 3's aren't magic numbers
            size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
            switch (tex_type)
            {
            case TextureType::Texture2D:
                glCompressedTexImage2D(texture_slot, level, format, width, height, 0, size, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureCLUT:
                glCompressedTexImage2D(texture_slot, level, format, width, height, 0, size, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureLuminance:
                glCompressedTexImage2D(texture_slot, level, format, width, height, 0, size, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureCube:
                // Just dump the same texture into all three sides of the cube
                for (uint8_t i = 0; i < 6; ++i)
                {
                    glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, format, width, height, 0, size, buffer + offset);
                    checkGLError();
                }
                break;
            default:
                throw std::runtime_error("AAARGH!");
            }
        }
        else
        {
            size = width * height;
            switch (tex_type)
            {
            case TextureType::Texture2D:
                glTexImage2D(texture_slot, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureCLUT:
                glTexImage2D(texture_slot, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureLuminance:
                glTexImage2D(texture_slot, level, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer + offset);
                checkGLError();
                break;
            case TextureType::TextureCube:
                // Just dump the same texture into all three sides of the cube
                for (uint8_t i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer + offset);
                    checkGLError();
                }
                break;
            default:
                throw std::runtime_error("AAARGH!");
            }
        }

        // offset is current byte offset from mip to map, as data is stored linearly
        offset += size;
        // assuming pow2 mipmaps
        width = width / 2;
        height = height / 2;
    }

    glBindTexture(texture_slot, 0);

    free(buffer);

    return tid;
}

struct DDSPixelFormat
{
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwFourCC;
    uint32_t dwRGBBitCount;
    uint32_t dwRBitMask;
    uint32_t dwGBitMask;
    uint32_t dwBBitMask;
    uint32_t dwABitMask;

};

struct DDSHeader
{
    uint32_t magicValue;
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwHeight;
    uint32_t dwWidth;
    uint32_t dwPitchOrLinearSize;
    uint32_t dwDepth;
    uint32_t dwMipMapCount;
    uint32_t dwReserved1[11];
    DDSPixelFormat ddspf;
    uint32_t dwCaps;
    uint32_t dwCaps2;
    uint32_t dwCaps3;
    uint32_t dwCaps4;
    uint32_t dwReserved2;
};


class DDSReader
{
public:
    DDSHeader header;
    std::vector<unsigned char> buf;
    DDSReader(const std::string& path)
    {
        auto rw = serialisation::ReadWriter<serialisation::readmode>(path);
        rw.readWriteData<DDSHeader, serialisation::LE>(header);
        auto buffer_size = (header.dwMipMapCount > 1 ? header.dwPitchOrLinearSize + header.dwPitchOrLinearSize : header.dwPitchOrLinearSize);

        buf.reserve(buffer_size);
        rw.readWriteBytes(buf);
    }
};
