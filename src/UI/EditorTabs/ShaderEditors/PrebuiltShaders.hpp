#pragma once

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

#include "Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "TabShaders.hpp"

class PrebuiltTab : public QWidget
{
	Q_OBJECT
private:
	typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
	typedef std::shared_ptr<Material> MaterialPtr;

	QListWidget* category_list = new QListWidget(this);
	QListWidget* shader_list = new QListWidget(this);
	QWidget* uniform_lists_widget = new QWidget(this);

	QGridLayout* vector_uniforms_layout = new QGridLayout;
	QWidget* vector_uniforms_widget = new QWidget(this->uniform_lists_widget);

	MaterialPtr selected_material = nullptr;
public:
	PrebuiltTab(TabShadersLibrary& tab_materials, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QVBoxLayout* layout = new QVBoxLayout(this);

		QWidget* lists_widget = new QWidget(this);
		QHBoxLayout* lists_layout = new QHBoxLayout(lists_widget);
		lists_widget->setLayout(lists_layout);
		lists_layout->addWidget(this->category_list);
		lists_layout->addWidget(this->shader_list);

		QVBoxLayout* uniform_lists_layout = new QVBoxLayout(uniform_lists_widget);
		this->uniform_lists_widget->setLayout(uniform_lists_layout);

		this->vector_uniforms_widget->setLayout(this->vector_uniforms_layout);

		uniform_lists_layout->addWidget(vector_uniforms_widget);

		layout->addWidget(lists_widget);
		layout->addWidget(uniform_lists_widget);
		this->setLayout(layout);

	}

	void updateDataList()
	{
		QLayoutItem* child;
		while ((child = this->vector_uniforms_layout->takeAt(0)) != 0) {
			delete child->widget();
			delete child;
		}

		for (uint8_t i=0; i < this->selected_material->material_uniforms.size(); ++i)
		{
			auto& uniform = this->selected_material->material_uniforms[i];
			uint8_t id = uniform->id;

			QWidget* tmp_widget = new QWidget(vector_uniforms_widget);
			QHBoxLayout* tmp_layout = new QHBoxLayout();
			tmp_widget->setLayout(tmp_layout);

			QLabel* tmp_label = new QLabel(tmp_widget);
			tmp_label->setText(QString::fromStdString(uniform->getName()));
			this->vector_uniforms_layout->addWidget(tmp_label, i, 0);

			std::array<float, 4>& static_val = this->selected_material->local_uniform_buffer[id];
		
			for (uint8_t j = 0; j < uniform->getSize(); ++j)
			{
				QLineEdit* tmp_line = new QLineEdit(tmp_widget);
				tmp_line->setText(QString::fromStdString(std::to_string(static_val[j])));
				tmp_layout->addWidget(tmp_line);
				connect(tmp_line, &QLineEdit::textChanged, this, [this, id, j](const QString& value) { this->sanitiseTextChanged(id, j, value); });
			}

			this->vector_uniforms_layout->addWidget(tmp_widget, i, 1);
		}

	}

	void sanitiseTextChanged(uint8_t id, uint8_t change_idx, const QString& value)
	{
		float f_value;
		try
		{
			f_value = std::stof(value.toStdString());
		}
		catch (const std::exception& ex)
		{
			f_value = 0;
		}
		std::array<float, 4>& static_val = this->selected_material->local_uniform_buffer[id];
		static_val[change_idx] = f_value;
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
