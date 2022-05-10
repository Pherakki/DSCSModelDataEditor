#pragma once

#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"

struct TabMaterials
{
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
public:
    MaterialPtr prebuilt_material = nullptr;
    MaterialPtr factory_material = nullptr;
    MaterialPtr custom_code_material = nullptr;
};

typedef std::vector<TabMaterials> TabMaterialLibrary;
