#pragma once

#include <QBoxLayout>
#include <QLabel>
#include <QWidget>

class TitleWidget : public QWidget
{
public:
	QLabel* label;

	explicit TitleWidget(const QString& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;

		layout->addStretch();
		this->label = new QLabel(label_text);
		layout->setAlignment(this->label, Qt::AlignCenter);
		layout->addWidget(this->label);
		layout->addStretch();

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};
