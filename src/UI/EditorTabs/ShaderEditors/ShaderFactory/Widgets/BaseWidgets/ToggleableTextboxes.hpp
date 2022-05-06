#pragma once

#include <cinttypes>

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QValidator>

class ToggleableTextboxesWidgetBase : public QWidget
{
	Q_OBJECT;
private:
	void toggleTextboxes(bool state)
	{
		for (auto*& box : this->textboxes)
			box->setEnabled(state);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	// This should be an array, but can't mix templates with Q_OBJECTs...
	std::vector<QLineEdit*> textboxes;

	explicit ToggleableTextboxesWidgetBase(uint8_t num_boxes, QString label_text, QWidget* parent = nullptr) 
		: QWidget(parent)
		, textboxes{ std::vector<QLineEdit*>(num_boxes) }
	{
		auto layout = new QHBoxLayout;

		this->checkbox = new QCheckBox;
		layout->addWidget(this->checkbox);
		layout->setAlignment(this->checkbox, Qt::AlignLeft);
		this->label = new QLabel(label_text);
		layout->addWidget(this->label);
		layout->setAlignment(this->label, Qt::AlignLeft);

		layout->addStretch();

		for (auto& box : this->textboxes)
		{
			box = new QLineEdit;
			box->setValidator(new QDoubleValidator(0, 100, 4, this));
			box->setMaximumWidth(40);
			layout->addWidget(box);
			layout->setAlignment(box, Qt::AlignRight);
		}

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		this->toggleTextboxes(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableTextboxesWidgetBase::toggleTextboxes);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &ToggleableTextboxesWidgetBase::settingsUpdated);
	}

signals:
	void settingsUpdated(bool);
};

// https://stackoverflow.com/questions/594730/overriding-static-variables-when-subclassing
template <uint8_t n_boxes>
class ToggleableTextboxesWidget : public ToggleableTextboxesWidgetBase
{
public:
	explicit ToggleableTextboxesWidget(QString label_text, QWidget* parent = nullptr)
		: ToggleableTextboxesWidgetBase(n_boxes, label_text, parent) {};
};
