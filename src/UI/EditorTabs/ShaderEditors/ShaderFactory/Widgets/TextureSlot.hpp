#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#include "BaseWidgets/FileCombobox.hpp"

class ShaderFactoryTextureSlot : public QWidget
{
	Q_OBJECT;
private:
	void toggle(bool active)
	{
		this->file_combo_box->setEnabled(active);
		this->uv_slot_combobox->setEnabled(active);
	}
signals:
	void settingsUpdated(bool);
public:
	QCheckBox* checkbox;
	QLabel* texture_label;
	FileComboBox* file_combo_box;
	QLabel* uv_slot_label;
	QComboBox* uv_slot_combobox;

	ShaderFactoryTextureSlot(QString label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
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

		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureSlot::settingsUpdated);
		connect(this->file_combo_box->combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryTextureSlot::settingsUpdated);
		connect(this->uv_slot_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryTextureSlot::settingsUpdated);
	}

	bool isActive()
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}
};
