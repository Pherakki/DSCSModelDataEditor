#pragma once

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include <UI/CgSyntaxHighlighter.hpp>

#include "TabShaders.hpp"
#include "UI/SelectedObjectReferences.hpp"

class CodeEditor : public QWidget
{
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;

    QVBoxLayout* layout = new QVBoxLayout(this);
    QTabWidget* code_tabs = new QTabWidget(this);
    QPlainTextEdit* vertex_shader_textedit = new QPlainTextEdit(this);
    QPlainTextEdit* fragment_shader_textedit = new QPlainTextEdit(this);
    QPushButton* compile_button = new QPushButton("Compile", this);

    SelectedObjectReferences& selected_objects;

    void setSyntaxHighlighter(QPlainTextEdit* textbox)
    {
        auto cg_highlighter = new cgSyntaxHighlighter(textbox->document());

        // Editor setup
        QFont font;
        font.setFamily("Courier");
        font.setStyleHint(QFont::Monospace);
        font.setFixedPitch(true);
        font.setPointSize(10);
        textbox->setFont(font);

        // https://stackoverflow.com/a/54605709
        static constexpr int tab_width_char = 4;
        const auto font_metrics = textbox->fontMetrics();

        // set the tab stop with double precision
        textbox->setTabStopDistance(tab_width_char * font_metrics.width(' '));
    }

    void updateUI()
    {
        auto& selected_material = this->selected_objects.getSelectedMaterial();
        this->vertex_shader_textedit->setPlainText(QString::fromStdString(selected_material->shader->vertex_source));
        this->fragment_shader_textedit->setPlainText(QString::fromStdString(selected_material->shader->fragment_source));
    }

public:
	CodeEditor(SelectedObjectReferences& sor, TabShadersLibrary& tab_materials, QWidget* parent=Q_NULLPTR) 
        : QWidget(parent)
        , selected_objects{sor}
	{
        this->setLayout(this->layout);
        this->setContentsMargins({ 0, 0, 0, 0 });
        this->layout->setContentsMargins({ 0, 0, 0, 0 });

        this->vertex_shader_textedit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
        this->setSyntaxHighlighter(this->vertex_shader_textedit);
        this->fragment_shader_textedit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
        this->setSyntaxHighlighter(this->fragment_shader_textedit);

        this->compile_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->code_tabs->addTab(this->vertex_shader_textedit, "Vertex");
        this->code_tabs->addTab(this->fragment_shader_textedit, "Fragment");
        this->layout->addWidget(this->code_tabs);
        this->layout->addWidget(this->compile_button);

        connect(&this->selected_objects, &SelectedObjectReferences::selectedMaterialUpdated, this, &CodeEditor::updateUI);
	}


};