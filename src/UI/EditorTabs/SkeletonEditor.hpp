#pragma once

#include <QtWidgets/QWidget>

#include "../../Renderer/src/DSCS/DataObjects/SkeletonDataBlocks.hpp"
#include "../../Renderer/src/DSCS/RenderObjects/Types.hpp"

class SkeletonEditorTab : QWidget
{
    Q_OBJECT
private:
    typedef DataBlocks::Skeleton::SkeletonDataBlock Skeleton;
    ModelList_t& models;

public:
    SkeletonEditorTab(ModelList_t& model_list) : models{ model_list } {};
};
