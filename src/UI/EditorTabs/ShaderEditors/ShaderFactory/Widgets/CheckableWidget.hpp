#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class CheckableWidget : public QWidget
{
private:
	QCheckBox* checkbox;
	QLayout* contents_layout = Q_NULLPTR;
	CheckableWidget(const QString& name, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		this->checkbox = new QCheckBox(this);
		auto label = new QLabel(name, this);

		auto _layout = new QGridLayout;
		_layout->addWidget(this->checkbox, 0, 0);
		_layout->addWidget(label, 0, 1);

		this->setLayout(_layout);
	}

public:
	void setContents(QLayout* layout)
	{
		this->contents_layout = layout;
		this->layout()->addItem(this->contents_layout, 1, 1);
	}

	void isActivated()
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}
};
