#pragma once

#include <unordered_map>

#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"

struct TabShaders
{
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
public:
    MaterialPtr prebuilt_material = nullptr;
    MaterialPtr factory_material = nullptr;
    MaterialPtr custom_code_material = nullptr;
};

typedef std::unordered_map<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>, TabShaders> ModelTabShadersLibrary;
typedef std::unordered_map<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel>, ModelTabShadersLibrary> TabShadersLibrary;
