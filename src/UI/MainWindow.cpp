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
    render_timeline_splitter->setSizes({ static_cast<int>(0.9f*render_timeline_splitter->size().height()), 
                                         static_cast<int>(0.1f*render_timeline_splitter->size().height()) });
    left_pane_layout->addWidget(render_timeline_splitter);
    left_pane->setLayout(left_pane_layout);



    // Create tabs for info view
    auto info_editor = new QTabWidget();
    auto mesh_info_tab = new MeshEditorTab(this);
    auto skeleton_info_tab = new QWidget(this);
    auto material_info_tab = new MaterialEditorTab(render_widget->texture_library, render_widget->shader_backend, this->render_widget->animation_buffer, this);
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

    // Model updates
    // -> Updated Widgets
    connect(this, &DSCSModelDataEditorWindow::selectedModelUpdated, mesh_info_tab, &MeshEditorTab::updateSelectedModel);
    connect(this, &DSCSModelDataEditorWindow::selectedModelUpdated, material_info_tab, &MaterialEditorTab::updateSelectedModel);

    // Mesh updates
    // -> Updating Widgets
    connect(mesh_info_tab, &MeshEditorTab::meshSelectionUpdated, this, &DSCSModelDataEditorWindow::setSelectedMesh);
    // -> Updated Widgets
    connect(this, &DSCSModelDataEditorWindow::selectedMeshUpdated, mesh_info_tab, &MeshEditorTab::updateSelectedMesh);
    connect(this, &DSCSModelDataEditorWindow::selectedMeshUpdated, material_info_tab, &MaterialEditorTab::updateSelectedMesh);

    // Material updates
    // -> Updating Widgets
    connect(mesh_info_tab, &MeshEditorTab::materialSelectionUpdated, this, &DSCSModelDataEditorWindow::setSelectedMaterial);
    connect(material_info_tab, &MaterialEditorTab::materialSelectionUpdated, this, &DSCSModelDataEditorWindow::setSelectedMaterial);
    connect(material_info_tab, &MaterialEditorTab::overwriteCurrentMaterial, this, &DSCSModelDataEditorWindow::overwriteMaterial);
    // -> Updated Widgets
    connect(this, &DSCSModelDataEditorWindow::selectedMaterialUpdated, mesh_info_tab, &MeshEditorTab::updateSelectedMaterial);
    connect(this, &DSCSModelDataEditorWindow::selectedMaterialUpdated, material_info_tab, &MaterialEditorTab::updateSelectedMaterial);
}

void DSCSModelDataEditorWindow::overwriteMaterial(MaterialPtr material)
{
    auto idx = -1;
    for (size_t i = 0; i < this->selected_model->materials.size(); ++i)
    {
        auto& s_material = this->selected_model->materials[i];
        if (this->selected_material == s_material)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        return;
    }
    else
    {
        auto s_mesh = this->selected_mesh;
        s_mesh->material = material;
        this->selected_model->materials[idx] = material;
        this->setSelectedModel(this->selected_model);
        this->setSelectedMesh(s_mesh);
    }
}

void DSCSModelDataEditorWindow::testInit()
{
    std::string test_path = "data/chr065";
    std::string anim_path2 = test_path;
    anim_path2 += "_bn01.anim";

    this->loadModel(QString::fromStdString(test_path));
    this->loadAnim(QString::fromStdString(anim_path2));
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
Global variable dispatchers
*/
void DSCSModelDataEditorWindow::setSelectedModelOnly(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model)
{
    this->selected_model = model;
    emit this->selectedModelUpdated(model);
}

void DSCSModelDataEditorWindow::setSelectedModel(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSModel> model)
{
    this->setSelectedModelOnly(model);
    if (model->meshes.size())
        this->setSelectedMesh(model->meshes[0]);
}

void DSCSModelDataEditorWindow::setSelectedMeshOnly(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh)
{
    this->selected_mesh = mesh;
    emit this->selectedMeshUpdated(mesh);
}

void DSCSModelDataEditorWindow::setSelectedMesh(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMesh> mesh)
{
    this->setSelectedMeshOnly(mesh);

    this->setSelectedMaterial(mesh->material);
}

void DSCSModelDataEditorWindow::setSelectedMaterial(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> material)
{
    this->selected_material = material;
    emit this->selectedMaterialUpdated(material);
}

/*
Asset loaders
*/
void DSCSModelDataEditorWindow::loadModel(const QString& fileName)
{
    auto model = this->render_widget->loadModel(fileName.toStdString());
    this->setSelectedModel(model);
}

void DSCSModelDataEditorWindow::loadAnim(const QString& fileName)
{
    this->render_widget->loadAnim(fileName.toStdString());
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
