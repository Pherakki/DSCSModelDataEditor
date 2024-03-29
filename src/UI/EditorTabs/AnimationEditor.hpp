#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "DSCSRenderer/src/DSCS/RenderObjects/Types.hpp"
#include "DSCSRenderer/src/DSCS/DataObjects/SkeletonDataBlocks.hpp"

class AnimationEditorTab : public QWidget
{
    Q_OBJECT
private:
    ModelID_t id;
    ModelList_t& models;
    void populateBoneViews();
public:
    AnimationEditorTab(ModelList_t& model_list);

    void setModel(ModelID_t id); 

};