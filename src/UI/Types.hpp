#pragma once

#include <unordered_map>
#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

typedef int32_t ModelID_t;
typedef std::unordered_map<ModelID_t, Rendering::DSCS::DataObjects::OpenGLDSCSModel> ModelList_t;
const ModelID_t null_model_id = (ModelID_t)(-1);