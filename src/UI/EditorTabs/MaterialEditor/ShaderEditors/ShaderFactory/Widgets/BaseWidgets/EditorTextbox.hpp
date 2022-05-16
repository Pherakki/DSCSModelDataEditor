#pragma once

#include <QDockWidget>
#include <QEvent>
#include <QFocusEvent>
#include <QLineEdit>
#include <QValidator>

class EditorTextbox : public QLineEdit
{
    Q_OBJECT
private:
    //void focusInEvent(QFocusEvent* event) override
    //{
    //    if (event->gotFocus())
    //    {
    //        //QDockWidget* dock = new QDockWidget("Title", this);

    //        //QWidget* central = new QWidget(this);
    //        //QVBoxLayout* l = new QVBoxLayout;
    //        //QLabel* testLabel = new QLabel("Test", central);
    //        //l->addWidget(testLabel);
    //        //central->setLayout(l);

    //        //dock->setWidget(central);
    //        //dock->setFloating(true);
    //        //dock->setWindowFlags(dock->windowFlags() | Qt::Tool | Qt::WindowStaysOnTopHint);

    //        //dock->show();
    //    }
    //    QLineEdit::focusInEvent(event);
    //}

    //void focusOutEvent(QFocusEvent* event) override
    //{
    //    if (event->lostFocus())
    //        this->setCursorPosition(0);
    //    QLineEdit::focusOutEvent(event);
    //}
public:
    EditorTextbox(QWidget* parent = Q_NULLPTR) : QLineEdit(parent)
    {
        this->setValidator(new QDoubleValidator(0, 100, 4, this));
        this->setMaximumWidth(40);
        this->setText("0.0");
    }
};

