#pragma once

#include "BaseWidgets/CheckableWidget.hpp"
#include "BaseWidgets/TextboxArrayWidget.hpp"

class GlassMapSettings : public CheckableWidget
{
	Q_OBJECT
public:
	TextboxArrayWidget<1>* glass_strength = new TextboxArrayWidget<1>("Strength", this);
	TextboxArrayWidget<1>* curvature      = new TextboxArrayWidget<1>("Curvature", this);

	GlassMapSettings(QWidget* parent = Q_NULLPTR) : CheckableWidget("Glass Map", parent)
	{
		auto contents_layout = new QVBoxLayout;
		{
			contents_layout->addWidget(this->glass_strength);
			contents_layout->addWidget(this->curvature);
		}
		this->setContents(contents_layout);

		connect(this, &CheckableWidget::stateChanged, this, &GlassMapSettings::settingsUpdated);
	}
signals:
	void settingsUpdated(bool);
};
