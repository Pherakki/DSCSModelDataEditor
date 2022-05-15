#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QValidator>

template <uint8_t n_boxes>
class TextboxArrayWidget : public QWidget
{
public:
	QLabel* label;
	std::array<QLineEdit*, n_boxes> textboxes;

	explicit TextboxArrayWidget(const QString& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;

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
			box->setText("0.0");
			layout->setAlignment(box, Qt::AlignRight);
		}

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}
};
