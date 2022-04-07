#pragma once

#include <unordered_map>
#include <memory>
#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

typedef int32_t ModelID_t;
typedef std::unordered_map<ModelID_t, std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel>> ModelList_t;
const ModelID_t null_model_id = (ModelID_t)(-1);