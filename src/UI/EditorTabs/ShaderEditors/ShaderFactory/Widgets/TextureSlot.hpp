#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#include "BaseWidgets/FileCombobox.hpp"

class ShaderFactoryTextureSlot : public QWidget
{
private:
	void toggle(bool active)
	{
		this->file_combo_box->setEnabled(active);
		this->uv_slot_combobox->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* texture_label;
	FileComboBox* file_combo_box;
	QLabel* uv_slot_label;
	QComboBox* uv_slot_combobox;

	ShaderFactoryTextureSlot(QString label_text, QWidget* parent = nullptr) : QWidget(parent)
	{
		auto _layout = new QHBoxLayout;

		this->checkbox = new QCheckBox;
		this->texture_label = new QLabel(label_text);
		this->file_combo_box = new FileComboBox;
		this->uv_slot_label = new QLabel("UV Slot:");
		this->uv_slot_combobox = new QComboBox;
		this->uv_slot_combobox->addItems({ "1", "2", "3" });

		_layout->addWidget(this->checkbox);
		_layout->addWidget(this->texture_label);
		_layout->addWidget(this->file_combo_box);
		_layout->addWidget(this->uv_slot_label);
		_layout->addWidget(this->uv_slot_combobox);

		this->setLayout(_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureSlot::toggle);
	}
};
