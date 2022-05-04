#pragma once

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QWidget>

class TextureMapWidget : public QWidget
{
private:
	void toggle(int active)
	{
		this->maptype->setEnabled(active);
		this->channel->setEnabled(active);
	}
public:
	QCheckBox* checkbox;
	QLabel* label;
	QComboBox* maptype;
	QComboBox* channel;

	TextureMapWidget(const QString& label_text, QWidget* parent = Q_NULLPTR) : QWidget(parent)
	{
		QHBoxLayout* _layout = new QHBoxLayout;
		{
			QHBoxLayout* title_layout = new QHBoxLayout;
			{
				this->checkbox = new QCheckBox(this);
				this->label = new QLabel(label_text, this);

				title_layout->addWidget(this->checkbox, 0, Qt::AlignLeft);
				auto spacer = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
				title_layout->addItem(spacer);
				title_layout->addWidget(this->label, 0, Qt::AlignLeft);
			}

			QHBoxLayout* contents_layout = new QHBoxLayout;
			QWidget* contents_widget = new QWidget(this);
			{
				this->maptype = new QComboBox(contents_widget);
				this->maptype->addItem("Diffuse");
				this->maptype->addItem("Normal");
				this->maptype->addItem("Light");

				this->channel = new QComboBox(contents_widget);
				this->channel->addItem("R");
				this->channel->addItem("G");
				this->channel->addItem("B");
				this->channel->addItem("A");

				contents_layout->addWidget(this->maptype, 0, Qt::AlignRight);
				contents_layout->addWidget(this->channel, 0, Qt::AlignRight);
				contents_widget->setLayout(contents_layout);
			}
			_layout->addItem(title_layout);
			_layout->addWidget(contents_widget, 1, Qt::AlignRight);
		}
		this->setLayout(_layout);

		this->toggle(false);
		connect(this->checkbox, &QCheckBox::stateChanged, this, &TextureMapWidget::toggle);
	}
};
