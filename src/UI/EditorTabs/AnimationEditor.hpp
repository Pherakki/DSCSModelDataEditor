#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "../../Renderer/DSCS/RenderObjects/Types.hpp"
#include "../../Renderer/DSCS/DataObjects/SkeletonDataBlocks.hpp"

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