#pragma once

#include <QtWidgets/QWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/SkeletonDataBlocks.hpp"

class SkeletonEditorTab : QWidget
{
    Q_OBJECT
private:
    typedef DataBlocks::Skeleton::SkeletonDataBlock Skeleton;
    ModelList_t& models;

public:
    SkeletonEditorTab(ModelList_t& model_list) : models{ model_list } {};
};