#pragma once

#include <map>
#include "../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"

typedef int32_t ModelID_t;
typedef std::map <ModelID_t, Rendering::DSCS::DataObjects::OpenGLDSCSModel> ModelList_t;
static const ModelID_t null_model_id = (ModelID_t)(-1);