#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/SkeletonDataBlocks.hpp"

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