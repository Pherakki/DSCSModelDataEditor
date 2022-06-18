#pragma once

#include <unordered_map>

#include "Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/Utils.hpp"

struct TabMaterials
{
private:
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;
public:
    MaterialPtr prebuilt_material = nullptr;
    MaterialPtr factory_material = nullptr;
    MaterialPtr custom_code_material = nullptr;
    FactorySettings settings;
};

typedef std::unordered_map<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>, TabMaterials> TabMaterialsLibrary;
