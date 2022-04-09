#pragma once

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

#include "Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"

class PrebuiltTab : public QWidget
{
	Q_OBJECT
private:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	QListWidget* category_list = new QListWidget(this);
	QListWidget* shader_list = new QListWidget(this);
	QVBoxLayout* vertex_uniforms_layout = new QVBoxLayout;
	QVBoxLayout* fragment_uniforms_layout = new QVBoxLayout;
	QVBoxLayout* texture_uniforms_layout = new QVBoxLayout;

	MaterialPtr selected_material = nullptr;
public:
	PrebuiltTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QVBoxLayout* layout = new QVBoxLayout(this);

		QWidget* lists_widget = new QWidget(this);
		QHBoxLayout* lists_layout = new QHBoxLayout(lists_widget);
		lists_widget->setLayout(lists_layout);
		lists_layout->addWidget(this->category_list);
		lists_layout->addWidget(this->shader_list);

		QWidget* uniform_lists_widget = new QWidget(this);
		QHBoxLayout* uniform_lists_layout = new QHBoxLayout(uniform_lists_widget);
		uniform_lists_widget->setLayout(uniform_lists_layout);

		QWidget* vertex_uniforms_widget = new QWidget(uniform_lists_widget);
		vertex_uniforms_widget->setLayout(this->vertex_uniforms_layout);
		QWidget* fragment_uniforms_widget = new QWidget(uniform_lists_widget);
		fragment_uniforms_widget->setLayout(this->fragment_uniforms_layout);
		QWidget* texture_uniforms_widget = new QWidget(uniform_lists_widget);
		texture_uniforms_widget->setLayout(this->texture_uniforms_layout);

		uniform_lists_layout->addWidget(vertex_uniforms_widget);
		uniform_lists_layout->addWidget(fragment_uniforms_widget);
		uniform_lists_layout->addWidget(texture_uniforms_widget);

		layout->addWidget(lists_widget);
		layout->addWidget(uniform_lists_widget);
		this->setLayout(layout);

	}

	void updateDataList()
	{
		for (auto& uniform : this->selected_material->material_uniforms)
		{
			std::cout << uniform->id << std::endl;
		}
	}

	/*
	Update methods for the data on this widget
	*/
	void updateSelectedMaterial(MaterialPtr material_ptr)
	{
		this->selected_material = material_ptr;
		this->updateDataList();
	}
};
