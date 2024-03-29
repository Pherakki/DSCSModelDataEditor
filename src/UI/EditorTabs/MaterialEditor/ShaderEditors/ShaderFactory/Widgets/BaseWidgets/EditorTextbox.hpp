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
    inline static const uint8_t num_digits = 4;
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
        auto validator = new QDoubleValidator(this);
        validator->setDecimals(this->num_digits);
        this->setValidator(validator);
        this->setMaximumWidth(40);
        this->setText("0.00");
    }
    auto getNumDigits()
    {
        return this->num_digits;
    }
};

