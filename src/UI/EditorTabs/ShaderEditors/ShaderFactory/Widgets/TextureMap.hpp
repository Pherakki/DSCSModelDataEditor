#pragma once

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QWidget>

//template <bool is_overlay>
class TextureMapWidget : public QWidget
{
private:
	QString diffuse_name = "Diffuse Texture";
	QString normal_name = "Normal Texture";
	QString light_name = "Light Texture";

	void toggle(int active)
	{
		this->maptype->setEnabled(active);
		this->channel->setEnabled(active);
	}

	void removeMap(const QString& name)
	{
		if (int idx = this->maptype->findText(name); idx != -1)
			this->maptype->removeItem(idx);
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

				this->channel = new QComboBox(contents_widget);
				this->channel->addItem("R", static_cast<int>(RGBAChannel::R));
				this->channel->addItem("G", static_cast<int>(RGBAChannel::G));
				this->channel->addItem("B", static_cast<int>(RGBAChannel::B));
				this->channel->addItem("A", static_cast<int>(RGBAChannel::A));

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

	void addDiffuseMap()
	{
		if (int idx = this->maptype->findText(this->diffuse_name); idx == -1)
			this->maptype->insertItem(0, this->diffuse_name, static_cast<int>(MapType::DIFFUSE));
	}

	void removeDiffuseMap()
	{
		this->removeMap(this->diffuse_name);
	}

	void addNormalMap()
	{
		if (int idx = this->maptype->findText(this->normal_name); idx == -1)
			this->maptype->insertItem(1, this->normal_name, static_cast<int>(MapType::NORMAL));
	}

	void removeNormalMap()
	{
		this->removeMap(this->normal_name);
	}

	void addLightMap()
	{
		if (int idx = this->maptype->findText(this->light_name); idx == -1)
			this->maptype->insertItem(2, this->light_name, static_cast<int>(MapType::LIGHT));
	}

	void removeLightMap()
	{
		this->removeMap(this->light_name);
	}

	MapType getMapType()
	{
		return static_cast<MapType>(this->maptype->currentData().toInt());
	}

	RGBAChannel getChannel()
	{
		return static_cast<RGBAChannel>(this->channel->currentData().toInt());
	}
};
