#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class CheckableWidget : public QWidget
{
protected:
	QCheckBox* checkbox;
	QGridLayout* _layout;
	QWidget* contents_widget;
	CheckableWidget(const QString& name, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		this->checkbox = new QCheckBox(this);
		auto label = new QLabel(name, this);

		this->_layout = new QGridLayout;
		this->_layout->addWidget(this->checkbox, 0, 0);
		this->_layout->addWidget(label, 0, 1);
		this->contents_widget = new QWidget(this);
		this->_layout->addWidget(this->contents_widget, 1, 1);

		this->setLayout(this->_layout);
		this->contents_widget->setEnabled(false);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &CheckableWidget::updateContentsEnableState);
	}

	void updateContentsEnableState()
	{
		this->contents_widget->setEnabled(this->checkbox->isChecked());
	}

public:
	void setContents(QLayout* layout)
	{
		this->contents_widget->setLayout(layout);
	}

	bool isActivated()
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}

	void toggle(bool active)
	{
		this->setEnabled(active);
		this->updateContentsEnableState();
	}
};
