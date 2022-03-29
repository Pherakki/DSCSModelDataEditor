#include "MainWindow.hpp"
#include "CgSyntaxHighlighter.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTabWidget>
#include <QScrollArea>



DSCSModelDataEditorWindow::DSCSModelDataEditorWindow(QWidget* parent = Q_NULLPTR) : QMainWindow(parent)
{
    // Qt5 takes ownership of raw pointers and handles destruction properly, 
    // so no memory leaks here even though it looks like it
    auto central_widget = new QWidget();
    auto main_layout = new QHBoxLayout;
    central_widget->setContentsMargins({ 0, 0, 0, 0 });
    main_layout->setContentsMargins({ 0, 0, 0, 0 });

    this->render_widget = new CustomWidgets::RenderWidget(this);
    this->render_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto animation_timeline = new QTextEdit;

    //////////////////////////////////////////////////////////////////////
    // TEMPORARY WIDGET LAYOUT LOGIC                                    //
    // SPLIT THESE OFF INTO SEPARATE FUNCTIONS TO MAKE IT MORE READABLE //
    //////////////////////////////////////////////////////////////////////

    // Organise the render widget and the animation timeline widgets
    auto left_pane = new QWidget;
    auto left_pane_layout = new QVBoxLayout;
    left_pane->setContentsMargins({ 0, 0, 0, 0 });
    left_pane_layout->setContentsMargins({ 0, 0, 0, 0 });

    auto render_timeline_splitter = new QSplitter;
    render_timeline_splitter->setContentsMargins({ 0, 0, 0, 0 });
    render_timeline_splitter->addWidget(render_widget);
    render_timeline_splitter->addWidget(animation_timeline);
    render_timeline_splitter->setOrientation(Qt::Vertical);
    render_timeline_splitter->setSizes({ static_cast<int>(0.9*render_timeline_splitter->size().height()), 
                                         static_cast<int>(0.1*render_timeline_splitter->size().height()) });
    left_pane_layout->addWidget(render_timeline_splitter);
    left_pane->setLayout(left_pane_layout);



    // Create tabs for info view
    auto info_editor = new QTabWidget();
    auto mesh_info_tab = new QWidget();
    auto skeleton_info_tab = new QWidget();

    auto material_info_tab = new QSplitter();
    material_info_tab->setContentsMargins({ 0, 0, 0, 0 });
    material_info_tab->setOrientation(Qt::Vertical);

    auto animation_info_tab = new AnimationEditorTab(render_widget->models);

    info_editor->addTab(animation_info_tab, "Animation");
    info_editor->addTab(mesh_info_tab, "Mesh");
    info_editor->addTab(material_info_tab, "Material");
    info_editor->addTab(skeleton_info_tab, "Skeleton");

    auto shader_editor = new QWidget;
    auto shader_editor_layout = new QVBoxLayout;
    shader_editor->setLayout(shader_editor_layout);
    shader_editor->setContentsMargins({ 0, 0, 0, 0 });
    shader_editor_layout->setContentsMargins({ 0, 0, 0, 0 });

    auto shader_editor_textedits = new QTabWidget();
    this->vertex_shader_textedit = new QPlainTextEdit();
    this->vertex_shader_textedit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    this->fragment_shader_textedit = new QPlainTextEdit();
    this->fragment_shader_textedit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    auto compile_button = new QPushButton("Compile");
    shader_editor_textedits->addTab(vertex_shader_textedit, "Vertex");
    shader_editor_textedits->addTab(fragment_shader_textedit, "Fragment");
    shader_editor_layout->addWidget(shader_editor_textedits);
    shader_editor_layout->addWidget(compile_button);


    auto shader_uniforms = new QWidget;
    auto shader_uniforms_layout = new QHBoxLayout;
    shader_uniforms->setLayout(shader_uniforms_layout);
    shader_uniforms->setContentsMargins({ 0, 0, 0, 0 });
    shader_uniforms_layout->setContentsMargins({ 0, 0, 0, 0 });

    auto vertex_uniforms = new QLabel("Vertex Uniforms");
    auto fragment_uniforms = new QLabel("Fragment Uniforms");
    auto texture_uniforms = new QLabel("Textures");
    shader_uniforms_layout->addWidget(vertex_uniforms);
    shader_uniforms_layout->addWidget(fragment_uniforms);
    shader_uniforms_layout->addWidget(texture_uniforms);

    material_info_tab->addWidget(shader_editor);
    material_info_tab->addWidget(shader_uniforms);

    auto scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setWidget(new ShaderFactory);
    material_info_tab->addWidget(scroll);

    // Syntax highlighting for Cg
    auto cg_highlighter_v = new cgSyntaxHighlighter(this->vertex_shader_textedit->document());
    auto cg_highlighter_f = new cgSyntaxHighlighter(this->fragment_shader_textedit->document());

    // Editor setup
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    this->vertex_shader_textedit->setFont(font);
    this->fragment_shader_textedit->setFont(font);

    // https://stackoverflow.com/a/54605709
    static constexpr int tab_width_char = 4;
    const auto font_metrics = vertex_shader_textedit->fontMetrics();


    // set the tab stop with double precision
    vertex_shader_textedit->setTabStopDistance(tab_width_char * font_metrics.width(' '));
    fragment_shader_textedit->setTabStopDistance(tab_width_char* font_metrics.width(' '));

    // Now put that group in a splitter with the info window
    auto left_right_pane_splitter = new QSplitter;
    left_right_pane_splitter->setContentsMargins({ 0, 0, 0, 0 });
    left_right_pane_splitter->addWidget(left_pane);
    left_right_pane_splitter->addWidget(info_editor);
    left_right_pane_splitter->setSizes({ static_cast<int>(0.8 * left_right_pane_splitter->size().width()),
                                         static_cast<int>(0.2 * left_right_pane_splitter->size().width()) });

    main_layout->addWidget(left_right_pane_splitter, 0, 0);

    this->setCentralWidget(central_widget);
    this->centralWidget()->setLayout(main_layout);

    // Set size to 70% of screen
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

    this->initToolbar();

    // Set up slots
    connect(this->render_widget, &CustomWidgets::RenderWidget::vertexShaderTextChanged, this, &DSCSModelDataEditorWindow::setVertexShaderText);
    connect(this->render_widget, &CustomWidgets::RenderWidget::fragmentShaderTextChanged, this, &DSCSModelDataEditorWindow::setFragmentShaderText);
}

void DSCSModelDataEditorWindow::openLoadModelDialog()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("DSCS Model Names (*.name)"));
    if (dialog.exec() == QDialog::Accepted)
    {
        QString name_file = dialog.selectedFiles().first();
        // Remove file extension
        this->loadModel(name_file.chopped(name_file.mid(name_file.lastIndexOf(".")).size()));
        dialog.close();

        QFileDialog dialog_anim(this);
        dialog_anim.setWindowModality(Qt::WindowModal);
        dialog_anim.setAcceptMode(QFileDialog::AcceptOpen);
        dialog_anim.setNameFilter(tr("DSCS Animations (*.anim)"));
        if (dialog_anim.exec() == QDialog::Accepted)
            this->loadAnim(dialog_anim.selectedFiles().first());
        dialog_anim.close();
    }
    else
    {
        dialog.close();
    }
}

void DSCSModelDataEditorWindow::loadModel(const QString& fileName)
{
    this->render_widget->loadModel(fileName.toStdString());
}

void DSCSModelDataEditorWindow::loadAnim(const QString& fileName)
{
    this->render_widget->loadAnim(fileName.toStdString());
}

void DSCSModelDataEditorWindow::initToolbar()
{
    QMenu* fileMenu = this->menuBar()->addMenu(tr("&File"));
    QAction* newAct = new QAction(tr("&Load Model"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Load a new model"));
    connect(newAct, &QAction::triggered, this, &DSCSModelDataEditorWindow::openLoadModelDialog);
    fileMenu->addAction(newAct);

    QMenu* helpMenu = this->menuBar()->addMenu(tr("&Help"));
    QAction* aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void DSCSModelDataEditorWindow::setVertexShaderText(const QString& shader_text)
{
    this->vertex_shader_textedit->setPlainText(shader_text);
}

void DSCSModelDataEditorWindow::setFragmentShaderText(const QString& shader_text)
{
    this->fragment_shader_textedit->setPlainText(shader_text);
}

#include "moc_MainWindow.cpp"