#pragma once

#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>

class FileComboBox : public QWidget
{
public:
	QComboBox* combobox;
	QPushButton* open_file_button;

	FileComboBox(QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto f_layout = new QHBoxLayout;

		this->combobox = new QComboBox;
		this->open_file_button = new QPushButton;

		f_layout->addWidget(this->combobox);
		f_layout->addWidget(this->open_file_button);

		this->setLayout(f_layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};
