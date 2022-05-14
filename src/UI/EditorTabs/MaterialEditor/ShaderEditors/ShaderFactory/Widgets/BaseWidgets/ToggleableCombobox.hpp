#pragma once

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QWidget>

class ToggleableCombobox : public QWidget
{
	Q_OBJECT
private:
	void toggle(int active)
	{
		this->blockSignals(true);
		this->combobox->setEnabled(active);
		this->blockSignals(false);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QComboBox* combobox;

	ToggleableCombobox(const std::string& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QHBoxLayout* _layout = new QHBoxLayout;
		{
			QHBoxLayout* title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox(this);
				this->label = new QLabel(QString::fromStdString(label_text), this);
				title_layout->addWidget(this->checkbox, 0, Qt::AlignLeft);
				auto verticalSpacer = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
				title_layout->addItem(verticalSpacer);
				title_layout->addWidget(this->label, 0, Qt::AlignLeft);
			}
			this->combobox = new QComboBox(this);

			_layout->addItem(title_layout);
			_layout->addWidget(this->combobox, 1, Qt::AlignRight);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableCombobox::toggle);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableCombobox::settingsUpdated);
		connect(this->combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&]() {this->settingsUpdated(true); });
	}
signals:
	void settingsUpdated(bool);
};