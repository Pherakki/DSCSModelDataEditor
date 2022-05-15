#pragma once

#include <QEvent>
#include <QLineEdit>
#include <QValidator>

class EditorTextbox : public QLineEdit
{
    Q_OBJECT
private:
    bool eventFilter(QObject* watched, QEvent* event) override
    {
        if (event->type() == QEvent::FocusOut)
        {
            this->setCursorPosition(0);
        }
        return false;
    }
public:
    EditorTextbox(QWidget* parent = Q_NULLPTR) : QLineEdit(parent)
    {
        this->setValidator(new QDoubleValidator(0, 100, 4, this));
        this->setMaximumWidth(40);
        this->setText("0.0");
    }
};

