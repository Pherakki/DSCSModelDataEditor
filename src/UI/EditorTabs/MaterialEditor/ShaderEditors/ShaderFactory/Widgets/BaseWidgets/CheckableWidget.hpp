#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

class CheckableWidget : public QWidget
{
	Q_OBJECT
protected:
	QCheckBox* checkbox;
	QGridLayout* _layout;
	QWidget* contents_widget;


	void updateContentsEnableState()
	{
		this->contents_widget->setEnabled(this->checkbox->isChecked());
	}

public:
	CheckableWidget(const QString& name, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		this->checkbox = new QCheckBox(this);
		auto label = new QLabel(name, this);

		this->_layout = new QGridLayout;
		this->_layout->addWidget(this->checkbox, 0, 0);
		this->_layout->addWidget(label, 0, 1, Qt::AlignLeft);
		this->contents_widget = new QWidget(this);
		this->_layout->addWidget(this->contents_widget, 1, 1, Qt::AlignLeft);

		this->_layout->setColumnStretch(0, 0);
		this->_layout->setColumnStretch(1, 1);
		this->contents_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

		this->setLayout(this->_layout);
		this->contents_widget->setEnabled(false);

		connect(this->checkbox, &QCheckBox::stateChanged, this, &CheckableWidget::updateContentsEnableState);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &CheckableWidget::stateChanged);
	}

	void setContents(QLayout* layout)
	{
		this->contents_widget->setLayout(layout);
	}

	bool isActive()
	{
		return this->checkbox->isEnabled() && this->checkbox->isChecked();
	}

	void toggle(bool active)
	{
		this->setEnabled(active);
		this->updateContentsEnableState();
	}
signals:
	void stateChanged(int);
};
