#include <QtCore/QCryptographicHash>
#include <QtCore/QDirIterator>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTabWidget>
#include <QScrollArea>

#include "MainWindow.hpp"
#include "CgSyntaxHighlighter.hpp"


typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;

DSCSModelDataEditorWindow::DSCSModelDataEditorWindow(QWidget* parent = Q_NULLPTR)
    : QMainWindow(parent)
    , render_widget{ new CustomWidgets::RenderWidget(this) }
    , selected_objects{SelectedObjectReferences(this->render_widget->animation_buffer, this->render_widget->texture_library)}
{
    // Qt5 takes ownership of raw pointers and handles destruction properly, 
    // so no memory leaks here even though it looks like it
    auto central_widget = new QWidget();
    auto main_layout = new QHBoxLayout;
    central_widget->setContentsMargins({ 0, 0, 0, 0 });
    main_layout->setContentsMargins({ 0, 0, 0, 0 });

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
    render_timeline_splitter->setSizes({ static_cast<int>(0.9f*render_timeline_splitter->size().height()), 
                                         static_cast<int>(0.1f*render_timeline_splitter->size().height()) });
    left_pane_layout->addWidget(render_timeline_splitter);
    left_pane->setLayout(left_pane_layout);



    // Create tabs for info view
    auto info_editor = new QTabWidget();
    auto mesh_info_tab = new MeshEditorTab(this->selected_objects, this);
    auto skeleton_info_tab = new QWidget(this);
    auto material_info_tab = new MaterialEditorTab(this->selected_objects, this->render_widget->shader_backend, this->render_widget->animation_buffer, this);
    auto animation_info_tab = new AnimationEditorTab(render_widget->models);

    info_editor->addTab(animation_info_tab, "Animation");
    info_editor->addTab(mesh_info_tab, "Mesh");
    info_editor->addTab(material_info_tab, "Material");
    info_editor->addTab(skeleton_info_tab, "Skeleton");

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
    connect(this->render_widget, &CustomWidgets::RenderWidget::glInitialised, this, &DSCSModelDataEditorWindow::testInit);

    this->loadShaderHashes("data/shaders");
}


void DSCSModelDataEditorWindow::testInit()
{
    std::string test_path = "data/chr065";
    std::string anim_path = test_path;
    anim_path += "_bn01.anim";

    this->loadModel(QString::fromStdString(test_path));
    if (std::filesystem::exists(anim_path))
        this->loadAnim(QString::fromStdString(anim_path));
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

/*
Asset loaders
*/
void DSCSModelDataEditorWindow::loadModel(const QString& fileName)
{
    auto model = this->render_widget->loadModel(fileName.toStdString());
    this->selected_objects.registerNewModel(model);
    this->selected_objects.setSelectedModel(model);
    if (model->meshes.size())
    {
        auto& mesh = model->meshes[0];
        this->selected_objects.setSelectedMesh(mesh);
        this->selected_objects.setSelectedMaterial(mesh->material);
    }
}

void DSCSModelDataEditorWindow::loadAnim(const QString& fileName)
{
    this->render_widget->loadAnim(fileName.toStdString());
}

void DSCSModelDataEditorWindow::loadShaderHashes(const QString& directory)
{
    QDirIterator shader_iterator(directory, { "*vp.shad" }, QDir::Files);
    while (shader_iterator.hasNext())
    {
        auto md5 = QCryptographicHash(QCryptographicHash::Algorithm::Md5);
        shader_iterator.next();
        // Hash vertex shader
        auto filepath = shader_iterator.filePath();
        auto str_path = filepath.toStdString();
        std::stringstream vp_file_contents;
        vp_file_contents << std::ifstream(str_path).rdbuf();
        auto vp_hash = md5.hash(vp_file_contents.str().c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();

        // Hash fragment shader
        filepath.replace(filepath.size() - 7, 1, "f");
        std::stringstream fp_file_contents;
        fp_file_contents << std::ifstream(filepath.toStdString()).rdbuf();
        auto fp_hash = md5.hash(fp_file_contents.str().c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();

        // Store hash
        auto shader_name = shader_iterator.fileName().toStdString();
        shader_name = shader_name.substr(0, shader_name.size() - 8);
        this->selected_objects.shader_hashes[vp_hash + fp_hash].push_back(shader_name);

        md5.reset();
    }  
}


/*
UI Init
*/
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


#include "moc_MainWindow.cpp"
