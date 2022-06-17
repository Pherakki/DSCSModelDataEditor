#pragma once

#include <QLabel>
#include <QWidget>
#include <QValidator>

#include "EditorTextbox.hpp"

template <size_t n_boxes>
class TextboxArrayWidget : public QWidget
{
public:
	QLabel* label;
	// Should be array, but need to define a consistent API...
	//std::array<EditorTextbox*, n_boxes> textboxes;
	std::vector<EditorTextbox*> textboxes;

	explicit TextboxArrayWidget(const QString& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		auto layout = new QHBoxLayout;
		this->textboxes.resize(n_boxes);

		this->label = new QLabel(label_text);
		layout->addWidget(this->label);
		layout->setAlignment(this->label, Qt::AlignLeft);

		layout->addStretch();

		for (auto& box : this->textboxes)
		{
			box = new EditorTextbox;
			layout->addWidget(box);
			layout->setAlignment(box, Qt::AlignRight);
		}

		this->setLayout(layout);
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	}

	bool isActive()
	{
		return this->checkbox->isChecked() && this->checkbox->isEnabled();
	}

};
