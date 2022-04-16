#pragma once

#include <unordered_map>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "ShaderEditors/ShaderEditorTabs.hpp"

enum class glBoolOptions
{
    enumGL_TRUE = GL_TRUE,
    enumGL_FALSE = GL_FALSE
};

enum class glBlendFuncOptions
{
    enumGL_ZERO                = GL_ZERO,
    enumGL_ONE                 = GL_ONE,
    enumGL_SRC_COLOR           = GL_SRC_COLOR,
    enumGL_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
    enumGL_SRC_ALPHA           = GL_SRC_ALPHA,
    enumGL_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
    enumGL_DST_ALPHA           = GL_DST_ALPHA,
    enumGL_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA
};

enum class glBlendEqOptions
{
    enumGL_FUNC_ADD              = GL_FUNC_ADD,
    enumGL_MIN                   = GL_MIN,
    enumGL_MAX                   = GL_MAX,
    enumGL_FUNC_SUBTRACT         = GL_FUNC_SUBTRACT,
    enumGL_FUNC_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT
};

enum class glCullFaceOptions
{
    enumGL_FRONT          = GL_FRONT,
    enumGL_BACK           = GL_BACK,
    enumGL_FRONT_AND_BACK = GL_FRONT_AND_BACK
};

enum class glCompOptions
{
    enumGL_NEVER    = GL_NEVER,
    enumGL_LESS     = GL_LESS,
    enumGL_EQUAL    = GL_EQUAL,
    enumGL_LEQUAL   = GL_LEQUAL,
    enumGL_GREATER  = GL_GREATER,
    enumGL_NOTEQUAL = GL_NOTEQUAL,
    enumGL_GEQUAL   = GL_GEQUAL,
    enumGL_ALWAYS   = GL_ALWAYS
};

namespace MatEditTypedefs
{
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;
    typedef std::vector<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLSettings::OpenGLSetting>> OGLS_t;
}


class OpenGLSettingsWidget : public QWidget
{
    Q_OBJECT
private:
    MatEditTypedefs::MaterialPtr selected_material = nullptr;

    QCheckBox* alphafunc_checkbox  = new QCheckBox(this);
    QCheckBox* depthtest_checkbox  = new QCheckBox(this);
    QCheckBox* blendfunc_checkbox  = new QCheckBox(this);
    QCheckBox* blendeq_checkbox    = new QCheckBox(this);
    QCheckBox* faceculling_checkbox = new QCheckBox(this);
    QCheckBox* depthfunc_checkbox  = new QCheckBox(this);
    QCheckBox* depthmask_checkbox  = new QCheckBox(this);
    QCheckBox* colormask_checkbox  = new QCheckBox(this);

    QComboBox* alphafunc_combobox     = new QComboBox(this);
    QComboBox* blendfunc_src_combobox = new QComboBox(this);
    QComboBox* blendfunc_dst_combobox = new QComboBox(this);
    QComboBox* blendeq_combobox       = new QComboBox(this);
    QComboBox* faceculling_combobox    = new QComboBox(this);
    QComboBox* depthfunc_combobox     = new QComboBox(this);
    QComboBox* depthmask_combobox     = new QComboBox(this);
    QComboBox* colormask_combobox_r   = new QComboBox(this);
    QComboBox* colormask_combobox_g   = new QComboBox(this);
    QComboBox* colormask_combobox_b   = new QComboBox(this);
    QComboBox* colormask_combobox_a   = new QComboBox(this);

    template<uint8_t setting_id, class FunctorT, int... active_ids>
    void handleCheckbox(int checkstate, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
    {
        auto& material = this->selected_material;
        if (!material)
            return;
        std::cout << "Checkbox ticked" << std::endl;
        auto& settings = material->opengl_settings;
        auto setting_idx = this->checkIfSettingExists(setting_id, settings);
        std::cout << "Found setting at " << std::to_string(setting_idx) << std::endl;

        // If ticked (or not ticked, depending on whether FunctorT is std::identity or std::logical_not)
        if (FunctorT()(checkstate))
        {
            std::cout << "Trying to add..." << std::endl;
            // If the setting doesn't exist...
            if (setting_idx == -1)
            {
                std::cout << "Added." << std::endl;
                material->addOpenGLSetting(setting_id, { v1, v2, v3, v4 });
            }
        }
        else
        {
            std::cout << "Checking if should erase..." << std::endl;

            // If any of the dependencies exist, don't delete
            if constexpr (sizeof...(active_ids))
            {
                for (const auto& id : { active_ids... })
                {
                    auto id_idx = this->checkIfSettingExists(id, settings);
                    if (id_idx != -1)
                    {
                        std::cout << "Hit early exit condition!" << std::endl;
                        return;
                    }
                }
            }

            // If no dependencies exist... check if it needs deleting
            if (setting_idx != -1)
            {
                std::cout << "Erased." << std::endl;
                settings.erase(settings.begin() + setting_idx);
            }
        }
    }

    int8_t checkIfSettingExists(uint8_t setting_id, const MatEditTypedefs::OGLS_t& settings)
    {
        for (int8_t idx = 0; idx < settings.size(); ++idx)
        {
            if (settings[idx]->getID() == setting_id)
                return idx;
        }
        return -1;
    }

public:
    OpenGLSettingsWidget(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QGridLayout(this);

        auto curr_row = 0;

        // Alpha Func
        auto alphafunc_label = new QLabel("Alpha Func", this);
        auto alphafunc_widget = new QWidget(this);
        auto alphafunc_layout = new QHBoxLayout;
        alphafunc_layout->addWidget(this->alphafunc_combobox);
        alphafunc_widget->setLayout(alphafunc_layout);
        layout->addWidget(this->alphafunc_checkbox, curr_row, 0);
        layout->addWidget(alphafunc_label, curr_row, 1);
        layout->addWidget(alphafunc_widget, curr_row, 2);
        connect(this->alphafunc_checkbox, &QCheckBox::stateChanged, this, 
            [this](int checkstate) 
            { 
                this->handleCheckbox<0xA0, std::identity>(checkstate, GL_NEVER, 0, 0, 0); // glAlphaFunc
                this->handleCheckbox<0xA1, std::identity>(checkstate, 1, 0, 0, 0); // glEnable(GL_ALPHA_TEST)
            }
        );
        ++curr_row;

        // glBlendFunc
        auto blendfunc_label = new QLabel("Blend Func", this);
        auto blendfunc_widget = new QWidget(this);
        auto blendfunc_layout = new QHBoxLayout;
        blendfunc_layout->addWidget(this->blendfunc_src_combobox);
        blendfunc_layout->addWidget(this->blendfunc_dst_combobox);
        blendfunc_widget->setLayout(blendfunc_layout);
        layout->addWidget(this->blendfunc_checkbox, curr_row, 0);
        layout->addWidget(blendfunc_label, curr_row, 1);
        layout->addWidget(blendfunc_widget, curr_row, 2);
        connect(this->blendfunc_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA2, std::identity>(checkstate, GL_ONE, GL_ZERO, 0, 0); // BlendFunc
                this->handleCheckbox<0xA4, std::identity, 0xA2, 0xA3>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        ++curr_row;

        // glBlendEquationSeparate
        auto blendeq_label = new QLabel("Blend Equation", this);
        auto blendeq_widget = new QWidget(this);
        auto blendeq_layout = new QHBoxLayout;
        blendeq_layout->addWidget(this->blendeq_combobox);
        blendeq_widget->setLayout(blendeq_layout);
        layout->addWidget(this->blendeq_checkbox, curr_row, 0);
        layout->addWidget(blendeq_label, curr_row, 1);
        layout->addWidget(blendeq_widget, curr_row, 2);
        connect(this->blendeq_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA3, std::identity>(checkstate, GL_FUNC_ADD, 0, 0, 0); // BlendEquationSeparate
                this->handleCheckbox<0xA4, std::identity, 0xA2, 0xA3>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        ++curr_row;


        // glCullFace
        auto faceculling_label = new QLabel("Face Culling", this);
        auto faceculling_widget = new QWidget(this);
        auto faceculling_layout = new QHBoxLayout;
        faceculling_layout->addWidget(this->faceculling_combobox);
        faceculling_widget->setLayout(faceculling_layout);
        layout->addWidget(this->faceculling_checkbox, curr_row, 0);
        layout->addWidget(faceculling_label, curr_row, 1);
        layout->addWidget(faceculling_widget, curr_row, 2);
        connect(this->faceculling_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA5, std::identity>(checkstate, GL_BACK, 0, 0, 0); // glCullFace
            }
        );
        ++curr_row;

        // Depth Test
        auto depthtest_label = new QLabel("Depth Test", this);
        layout->addWidget(this->depthtest_checkbox, curr_row, 0);
        layout->addWidget(depthtest_label, curr_row, 1);
        ++curr_row;

        // glDepthFunc
        auto depthfunc_label = new QLabel("Depth Function", this);
        auto depthfunc_widget = new QWidget(this);
        auto depthfunc_layout = new QHBoxLayout;
        depthfunc_layout->addWidget(this->depthfunc_combobox);
        depthfunc_widget->setLayout(depthfunc_layout);
        layout->addWidget(this->depthfunc_checkbox, curr_row, 0);
        layout->addWidget(depthfunc_label, curr_row, 1);
        layout->addWidget(depthfunc_widget, curr_row, 2);
        ++curr_row;

        // glDepthMask
        auto depthmask_label = new QLabel("Depth Mask", this);
        auto depthmask_widget = new QWidget(this);
        auto depthmask_layout = new QHBoxLayout;
        depthmask_layout->addWidget(this->depthmask_combobox);
        depthmask_widget->setLayout(depthmask_layout);
        layout->addWidget(this->depthmask_checkbox, curr_row, 0);
        layout->addWidget(depthmask_label, curr_row, 1);
        layout->addWidget(depthmask_widget, curr_row, 2);
        ++curr_row;

        // glColorMask
        auto colormask_label = new QLabel("Color Mask", this);
        auto colormask_widget = new QWidget(this);
        auto colormask_layout = new QHBoxLayout;
        colormask_layout->addWidget(this->colormask_combobox_r);
        colormask_layout->addWidget(this->colormask_combobox_g);
        colormask_layout->addWidget(this->colormask_combobox_b);
        colormask_layout->addWidget(this->colormask_combobox_a);
        colormask_widget->setLayout(colormask_layout);
        layout->addWidget(this->colormask_checkbox, curr_row, 0);
        layout->addWidget(colormask_label, curr_row, 1);
        layout->addWidget(colormask_widget, curr_row, 2);
        ++curr_row;

        layout->setColumnStretch(0, 0);
        layout->setColumnStretch(1, 0);
        layout->setColumnStretch(2, 1);
        this->setLayout(layout);
    }

    void updateSelectedMaterial(MatEditTypedefs::MaterialPtr material_ptr)
    {
        this->selected_material = material_ptr;
    }
};

class MaterialEditorTab : public QWidget
{
    Q_OBJECT
private:
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSModel Model;
    typedef std::shared_ptr<Model> ModelPtr;
    typedef Rendering::DSCS::DataObjects::OpenGLDSCSMaterial Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef void(QComboBox::* CIC_t)(int);

    ModelPtr selected_model = nullptr;
    MaterialPtr selected_material = nullptr;
    // Widgets
    QComboBox* material_dropdown;
    ShaderEditorTabs* shader_edit_modes;
    OpenGLSettingsWidget* opengl_settings;

    // Lookup dicts
    std::unordered_map<MaterialPtr, int> material_lookup;
    std::unordered_map<int, MaterialPtr> material_reverse_lookup;

public:
    MaterialEditorTab(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QVBoxLayout(this);
        this->material_dropdown = new QComboBox(this);
        this->shader_edit_modes = new ShaderEditorTabs(this);
        this->opengl_settings = new OpenGLSettingsWidget(this);

        layout->addWidget(this->material_dropdown);
        layout->addWidget(this->shader_edit_modes);
        layout->addWidget(this->opengl_settings);
        this->setLayout(layout);

        connect(this->material_dropdown, static_cast<CIC_t>(&QComboBox::currentIndexChanged), this, &MaterialEditorTab::selectMaterial);
    };

    void updateDataList()
    {
        this->material_dropdown->clear();
        this->material_lookup.clear();
        this->material_reverse_lookup.clear();
        if (this->selected_model)
        {
            for (size_t i = 0; i < this->selected_model->materials.size(); ++i)
            {
                auto& material = this->selected_model->materials[i];
                this->material_dropdown->addItem(QString::fromStdString(material->name), i);
                this->material_lookup[material] = i;
                this->material_reverse_lookup[i] = material;
            }
        }
    }

signals:
    void materialSelectionUpdated(MaterialPtr material_ptr);
public slots:
    /*
    Update methods for the data on this widget
    */
    void updateSelectedModel(ModelPtr model_ptr)
    {
        this->selected_model = model_ptr;
        this->updateDataList();
    }

    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->material_dropdown->setCurrentIndex(this->material_lookup.at(material_ptr));
        this->selected_material = selected_material;
        this->shader_edit_modes->updateSelectedMaterial(material_ptr);
        this->opengl_settings->updateSelectedMaterial(material_ptr);
    }

    /*
    Relay methods up to the central signal dispatcher for global variable changes
    */
    void selectMaterial(int index)
    {
        if (!this->material_reverse_lookup.contains(index))
            return;
        auto& material = this->material_reverse_lookup.at(index);
        emit this->materialSelectionUpdated(material);
    }

};