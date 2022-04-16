#pragma once

#include <unordered_map>

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "ShaderEditors/ShaderEditorTabs.hpp"

struct glOptions
{
    const static std::vector<std::string> names;
    const static std::vector<uint16_t> options;
};

struct glBoolOptions : public glOptions
{
    inline const static std::vector<std::string> names   = { "GL_TRUE", "GL_FALSE" };
    inline const static std::vector<uint16_t> options    = {  GL_TRUE ,  GL_FALSE  };
};

struct glBlendFuncOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_ZERO", "GL_ONE", "GL_SRC_COLOR", "GL_ONE_MINUS_SRC_COLOR", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA", "GL_DST_COLOR", "GL_ONE_MINUS_DST_COLOR", "GL_DST_ALPHA", "GL_ONE_MINUS_DST_ALPHA", "GL_SRC_ALPHA_SATURATE" };
    inline const static std::vector<uint16_t> options  = {  GL_ZERO ,  GL_ONE ,  GL_SRC_COLOR ,  GL_ONE_MINUS_SRC_COLOR ,  GL_SRC_ALPHA ,  GL_ONE_MINUS_SRC_ALPHA ,  GL_DST_COLOR ,  GL_ONE_MINUS_DST_COLOR ,  GL_DST_ALPHA ,  GL_ONE_MINUS_DST_ALPHA ,  GL_SRC_ALPHA_SATURATE  };
};

struct glBlendEqOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_FUNC_ADD", "GL_MIN", "GL_MAX", "GL_FUNC_SUBTRACT", "GL_FUNC_REVERSE_SUBTRACT" };
    inline const static std::vector<uint16_t> options  = {  GL_FUNC_ADD ,  GL_MIN ,  GL_MAX ,  GL_FUNC_SUBTRACT ,  GL_FUNC_REVERSE_SUBTRACT  };
};


struct glCullFaceOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_FRONT", "GL_BACK", "GL_FRONT_AND_BACK" };
    inline const static std::vector<uint16_t> options  = {  GL_FRONT ,  GL_BACK ,  GL_FRONT_AND_BACK  };
};

struct glCompOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_NEVER", "GL_LESS", "GL_LEQUAL", "GL_EQUAL", "GL_NOTEQUAL", "GL_GEQUAL", "GL_GREATER", "GL_ALWAYS" };
    inline const static std::vector<uint16_t> options  = {  GL_NEVER ,  GL_LESS ,  GL_LEQUAL ,  GL_EQUAL ,  GL_NOTEQUAL,   GL_GEQUAL ,  GL_GREATER ,  GL_ALWAYS  };

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

    QCheckBox* alphafunc_checkbox   = new QCheckBox(this);
    QCheckBox* blendfunc_checkbox   = new QCheckBox(this);
    QCheckBox* blendeq_checkbox     = new QCheckBox(this);
    QCheckBox* faceculling_checkbox = new QCheckBox(this);
    QCheckBox* depthfunc_checkbox   = new QCheckBox(this);
    QCheckBox* depthmask_checkbox   = new QCheckBox(this);
    QCheckBox* colormask_checkbox   = new QCheckBox(this);

    QComboBox* alphafunc_combobox     = new QComboBox(this);
    QComboBox* blendfunc_src_combobox = new QComboBox(this);
    QComboBox* blendfunc_dst_combobox = new QComboBox(this);
    QComboBox* blendeq_combobox       = new QComboBox(this);
    QComboBox* faceculling_combobox   = new QComboBox(this);
    QComboBox* depthfunc_combobox     = new QComboBox(this);
    QComboBox* depthmask_combobox     = new QComboBox(this);
    QComboBox* colormask_combobox_r   = new QComboBox(this);
    QComboBox* colormask_combobox_g   = new QComboBox(this);
    QComboBox* colormask_combobox_b   = new QComboBox(this);
    QComboBox* colormask_combobox_a   = new QComboBox(this);


    QLineEdit* alphafunc_textbox = new QLineEdit(this);

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

    template<typename F1, typename F2, typename F3, typename F4>
    void updateSettingData(uint8_t setting_id, F1 f1, F2 f2, F3 f3, F4 f4)
    {
        auto& material = this->selected_material;
        auto& settings = material->opengl_settings;
        auto setting_idx = this->checkIfSettingExists(setting_id, settings);
        if (setting_idx != -1)
        {
            auto& setting = material->opengl_settings[setting_idx];
            setting->setData({ f1(), f2(), f3(), f4() });
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

    std::string sanitiseFloatText(const QString& value)
    {
        float f_value;
        try
        {
            f_value = std::stof(value.toStdString());
            return value.toStdString();
        }
        catch (const std::exception& ex)
        {
            return std::string("0.");
        }
    }

    void updateAlphaFunc()
    {
        this->updateSettingData
        (
            0xA0,
            [this]() { return this->alphafunc_combobox->currentData().toString().toStdString(); },
            [this]() { return this->sanitiseFloatText(this->alphafunc_textbox->text()); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateBlendFunc()
    {
        this->updateSettingData
        (
            0xA2,
            [this]() { return this->blendfunc_src_combobox->currentData().toString().toStdString(); },
            [this]() { return this->blendfunc_dst_combobox->currentData().toString().toStdString(); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateBlendEq()
    {
        this->updateSettingData
        (
            0xA3,
            [this]() { return this->blendeq_combobox->currentData().toString().toStdString(); },
            []() { return std::string(""); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateCullFace()
    {
        this->updateSettingData
        (
            0xA5,
            [this]() { return this->faceculling_combobox->currentData().toString().toStdString(); },
            []() { return std::string(""); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateDepthFunc()
    {
        this->updateSettingData
        (
            0xA7,
            [this]() { return this->depthfunc_combobox->currentData().toString().toStdString(); },
            []() { return std::string(""); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateDepthMask()
    {
        this->updateSettingData
        (
            0xA8,
            [this]() { return this->depthmask_combobox->currentData().toString().toStdString(); },
            []() { return std::string(""); },
            []() { return std::string(""); },
            []() { return std::string(""); }
        );
    }

    void updateColorMask()
    {
        this->updateSettingData
        (
            0xAC,
            [this]() { return this->colormask_combobox_r->currentData().toString().toStdString(); },
            [this]() { return this->colormask_combobox_g->currentData().toString().toStdString(); },
            [this]() { return this->colormask_combobox_b->currentData().toString().toStdString(); },
            [this]() { return this->colormask_combobox_a->currentData().toString().toStdString(); }
        );
    }

    template <typename option_pack>
    requires std::is_base_of<glOptions, option_pack>::value
    void initComboBox(QComboBox* combobox)
    {
        for (size_t i = 0; i < option_pack::names.size(); ++i)
        {
            combobox->addItem(QString::fromStdString(option_pack::names[i]), option_pack::options[i]);
        }
    }


public:
    OpenGLSettingsWidget(QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        auto layout = new QGridLayout(this);

        auto curr_row = 0;

        // Would probably be more readable to hide each chunk away in a constructor function

        // Alpha Func
        // -> Init UI
        auto alphafunc_label = new QLabel("Alpha Func", this);
        auto alphafunc_widget = new QWidget(this);
        auto alphafunc_layout = new QHBoxLayout;
        alphafunc_layout->addWidget(this->alphafunc_combobox);
        alphafunc_layout->addWidget(this->alphafunc_textbox);
        alphafunc_widget->setLayout(alphafunc_layout);
        layout->addWidget(this->alphafunc_checkbox, curr_row, 0);
        layout->addWidget(alphafunc_label, curr_row, 1);
        layout->addWidget(alphafunc_widget, curr_row, 2);
        // -> Init behaviour
        this->initComboBox<glCompOptions>(this->alphafunc_combobox);
        connect(this->alphafunc_checkbox, &QCheckBox::stateChanged, this, 
            [this](int checkstate) 
            { 
                this->handleCheckbox<0xA0, std::identity>(checkstate, GL_NEVER, 0, 0, 0); // glAlphaFunc
                this->handleCheckbox<0xA1, std::identity>(checkstate,  GL_TRUE, 0, 0, 0); // glEnable(GL_ALPHA_TEST)
            }
        );
        connect(this->alphafunc_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateAlphaFunc(); });
        connect(this->alphafunc_textbox,  &QLineEdit::textChanged, this, [this]() {this->updateAlphaFunc(); });
        ++curr_row;

        // glBlendFunc
        // -> Init UI
        auto blendfunc_label = new QLabel("Blend Func", this);
        auto blendfunc_widget = new QWidget(this);
        auto blendfunc_layout = new QHBoxLayout;
        blendfunc_layout->addWidget(this->blendfunc_src_combobox);
        blendfunc_layout->addWidget(this->blendfunc_dst_combobox);
        blendfunc_widget->setLayout(blendfunc_layout);
        layout->addWidget(this->blendfunc_checkbox, curr_row, 0);
        layout->addWidget(blendfunc_label, curr_row, 1);
        layout->addWidget(blendfunc_widget, curr_row, 2);
        // -> Init behaviour
        this->initComboBox<glBlendFuncOptions>(this->blendfunc_src_combobox);
        this->initComboBox<glBlendFuncOptions>(this->blendfunc_dst_combobox);
        connect(this->blendfunc_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA2, std::identity>(checkstate, GL_ONE, GL_ZERO, 0, 0); // BlendFunc
                this->handleCheckbox<0xA4, std::identity, 0xA2, 0xA3>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        connect(this->blendfunc_src_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateBlendFunc(); });
        connect(this->blendfunc_dst_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateBlendFunc(); });
        ++curr_row;

        // glBlendEquationSeparate
        // -> Init UI
        auto blendeq_label = new QLabel("Blend Equation", this);
        auto blendeq_widget = new QWidget(this);
        auto blendeq_layout = new QHBoxLayout;
        blendeq_layout->addWidget(this->blendeq_combobox);
        blendeq_widget->setLayout(blendeq_layout);
        layout->addWidget(this->blendeq_checkbox, curr_row, 0);
        layout->addWidget(blendeq_label, curr_row, 1);
        layout->addWidget(blendeq_widget, curr_row, 2);
        // -> Init behaviour
        this->initComboBox<glBlendEqOptions>(this->blendeq_combobox);
        connect(this->blendeq_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA3, std::identity>(checkstate, GL_FUNC_ADD, 0, 0, 0); // BlendEquationSeparate
                this->handleCheckbox<0xA4, std::identity, 0xA2, 0xA3>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        connect(this->blendeq_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateBlendEq(); });
        ++curr_row;


        // glCullFace
        // -> Init UI
        auto faceculling_label = new QLabel("Face Culling", this);
        auto faceculling_widget = new QWidget(this);
        auto faceculling_layout = new QHBoxLayout;
        faceculling_layout->addWidget(this->faceculling_combobox);
        faceculling_widget->setLayout(faceculling_layout);
        layout->addWidget(this->faceculling_checkbox, curr_row, 0);
        layout->addWidget(faceculling_label, curr_row, 1);
        layout->addWidget(faceculling_widget, curr_row, 2);
        // -> Init behaviour
        this->initComboBox<glCullFaceOptions>(this->faceculling_combobox);
        connect(this->faceculling_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA5, std::identity>(checkstate, GL_BACK, 0, 0, 0); // glCullFace
            }
        );
        connect(this->faceculling_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateCullFace(); });
        ++curr_row;

        // glDepthFunc
        // -> Init UI
        auto depthfunc_label = new QLabel("Depth Function", this);
        auto depthfunc_widget = new QWidget(this);
        auto depthfunc_layout = new QHBoxLayout;
        depthfunc_layout->addWidget(this->depthfunc_combobox);
        depthfunc_widget->setLayout(depthfunc_layout);
        layout->addWidget(this->depthfunc_checkbox, curr_row, 0);
        layout->addWidget(depthfunc_label, curr_row, 1);
        layout->addWidget(depthfunc_widget, curr_row, 2);
        // Init behaviour
        this->initComboBox<glCompOptions>(this->depthfunc_combobox);
        connect(this->depthfunc_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA7, std::identity>(checkstate, GL_LESS, 0, 0, 0); // glDepthFunc
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, 0xA7, 0xA8>(checkstate, 1, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
            }
        );
        connect(this->depthfunc_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateDepthFunc(); });
        ++curr_row;

        // glDepthMask
        // -> Init UI
        auto depthmask_label = new QLabel("Depth Mask", this);
        auto depthmask_widget = new QWidget(this);
        auto depthmask_layout = new QHBoxLayout;
        depthmask_layout->addWidget(this->depthmask_combobox);
        depthmask_widget->setLayout(depthmask_layout);
        layout->addWidget(this->depthmask_checkbox, curr_row, 0);
        layout->addWidget(depthmask_label, curr_row, 1);
        layout->addWidget(depthmask_widget, curr_row, 2);
        // -> Init behaviour
        this->initComboBox<glBoolOptions>(this->depthmask_combobox);
        connect(this->depthmask_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xA8, std::identity>(checkstate, GL_TRUE, 0, 0, 0); // glDepthMask
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, 0xA7, 0xA8>(checkstate, 1, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
            }
        );
        connect(this->depthmask_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateDepthMask(); });
        ++curr_row;

        // glColorMask
        // -> Init UI
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
        // -> Init behaviour
        this->initComboBox<glBoolOptions>(this->colormask_combobox_r);
        this->initComboBox<glBoolOptions>(this->colormask_combobox_g);
        this->initComboBox<glBoolOptions>(this->colormask_combobox_b);
        this->initComboBox<glBoolOptions>(this->colormask_combobox_a);
        connect(this->colormask_checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<0xAC, std::identity>(checkstate, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // glColorMask
            }
        );
        connect(this->colormask_combobox_r, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateColorMask(); });
        connect(this->colormask_combobox_g, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateColorMask(); });
        connect(this->colormask_combobox_b, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateColorMask(); });
        connect(this->colormask_combobox_a, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateColorMask(); });
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