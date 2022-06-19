#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#include "BaseWidgets/FileCombobox.hpp"


class ShaderFactoryTextureSlotNoUV : public QWidget
{
	Q_OBJECT;
private:
	void toggle(bool active)
	{
		this->blockSignals(true);
		this->file_combo_box->setEnabled(active);
		this->blockSignals(false);
	}
signals:
	void settingsUpdated(bool);
public:
	QCheckBox* checkbox;
	QLabel* texture_label;
	FileComboBox* file_combo_box;

	ShaderFactoryTextureSlotNoUV(QString label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QHBoxLayout;

		this->checkbox = new QCheckBox;
		this->texture_label = new QLabel(label_text);
		this->file_combo_box = new FileComboBox;

		_layout->addWidget(this->checkbox);
		_layout->addWidget(this->texture_label);
		_layout->addWidget(this->file_combo_box);

		this->setLayout(_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureSlotNoUV::toggle);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &ShaderFactoryTextureSlotNoUV::settingsUpdated);
		connect(this->file_combo_box->combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryTextureSlotNoUV::settingsUpdated);
	}

	bool isActive()
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}
};

class ShaderFactoryComboboxTextureSlot : public QWidget
{
	Q_OBJECT;

signals:
	void settingsUpdated(bool);
public:
	void toggle(bool active)
	{
		this->file_combo_box->setEnabled(active);
		this->uv_slot_combobox->setEnabled(active);
	}
	QComboBox* combobox;
	FileComboBox* file_combo_box;
	QLabel* uv_slot_label;
	QComboBox* uv_slot_combobox;

	ShaderFactoryComboboxTextureSlot(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto _layout = new QHBoxLayout;

		this->combobox = new QComboBox(this);
		this->file_combo_box = new FileComboBox(this);
		this->uv_slot_label = new QLabel("UV Slot:", this);
		this->uv_slot_combobox = new QComboBox(this);
		this->uv_slot_combobox->addItems({ "1", "2", "3" });

		_layout->addWidget(this->combobox);
		_layout->addWidget(this->file_combo_box);
		_layout->addWidget(this->uv_slot_label);
		_layout->addWidget(this->uv_slot_combobox);

		this->setLayout(_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		this->toggle(false);

		connect(this->combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryComboboxTextureSlot::settingsUpdated);
		connect(this->file_combo_box->combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryComboboxTextureSlot::settingsUpdated);
		connect(this->uv_slot_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ShaderFactoryComboboxTextureSlot::settingsUpdated);
	}

	bool isActive()
	{
		return this->isEnabled();
	}
};

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

	bool isActive() const
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}
};
