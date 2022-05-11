#pragma once

#include <unordered_map>

#include "Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

struct TabShaders
{
private:
    typedef std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject> ShaderPtr;
public:
    ShaderPtr prebuilt_shader = nullptr;
    ShaderPtr factory_shader = nullptr;
    ShaderPtr custom_code_shader = nullptr;
};

typedef std::unordered_map<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>, TabShaders> ModelTabShadersLibrary;
typedef std::unordered_map<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel>, ModelTabShadersLibrary> TabShadersLibrary;
