#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QTreeView>

#include "AnimationEditor.hpp"


AnimationEditorTab::AnimationEditorTab(ModelList_t& model_list) 
	: models{ model_list }
	, id    { null_model_id }
{
	auto layout = new QVBoxLayout;

	auto bone_view_holder = new QWidget;
	auto bone_view_holder_layout = new QHBoxLayout;
	bone_view_holder->setLayout(bone_view_holder_layout);
	auto bone_list = new QListView;
	auto bone_tree = new QTreeView;
	bone_view_holder_layout->addWidget(bone_list);
	bone_view_holder_layout->addWidget(bone_tree);

	layout->addWidget(bone_view_holder);
	this->setLayout(layout);
}


void AnimationEditorTab::setModel(ModelID_t id)
{

}


void AnimationEditorTab::populateBoneViews()
{
	auto& model = this->models.at(this->id);
	auto& bones = model.skeleton.getBoneDataBlocks();
}