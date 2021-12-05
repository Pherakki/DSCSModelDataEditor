#include "MaterialReadWrite.hpp"

namespace FileFormats::DSCS::GeomFile
{
    // initMemberVectorsIfRequired
    template<>
    void MaterialReadWrite::initMemberVectorsIfRequired<serialisation::readmode>()
    {
        this->shader_uniforms = std::vector<ShaderUniform>(this->num_shader_uniforms);
        this->opengl_settings = std::vector<OpenGLSetting>(this->num_OpenGL_settings);
    }
    template<>
    void MaterialReadWrite::initMemberVectorsIfRequired<serialisation::writemode>() {}

}