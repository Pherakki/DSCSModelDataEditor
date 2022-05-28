#pragma once

#include <QComboBox>
#include <QLineEdit>
#include <QValidator>
#include <QWidget>

#include "../../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
#include "UI/SelectedObjectReferences.hpp"

struct identity
{
    template <class T>
    constexpr T&& operator()(T&& t) const noexcept { return std::forward<T>(t); }
};

struct glOptions
{
    const static std::vector<std::string> names;
    const static std::vector<uint16_t> options;
};

struct glBoolOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_TRUE", "GL_FALSE" };
    inline const static std::vector<uint16_t> options = { GL_TRUE ,  GL_FALSE };
};

struct glBlendFuncOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_ZERO", "GL_ONE", "GL_SRC_COLOR", "GL_ONE_MINUS_SRC_COLOR", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA", "GL_DST_COLOR", "GL_ONE_MINUS_DST_COLOR", "GL_DST_ALPHA", "GL_ONE_MINUS_DST_ALPHA", "GL_SRC_ALPHA_SATURATE" };
    inline const static std::vector<uint16_t> options = { GL_ZERO ,  GL_ONE ,  GL_SRC_COLOR ,  GL_ONE_MINUS_SRC_COLOR ,  GL_SRC_ALPHA ,  GL_ONE_MINUS_SRC_ALPHA ,  GL_DST_COLOR ,  GL_ONE_MINUS_DST_COLOR ,  GL_DST_ALPHA ,  GL_ONE_MINUS_DST_ALPHA ,  GL_SRC_ALPHA_SATURATE };
};

struct glBlendEqOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_FUNC_ADD", "GL_MIN", "GL_MAX", "GL_FUNC_SUBTRACT", "GL_FUNC_REVERSE_SUBTRACT" };
    inline const static std::vector<uint16_t> options = { GL_FUNC_ADD ,  GL_MIN ,  GL_MAX ,  GL_FUNC_SUBTRACT ,  GL_FUNC_REVERSE_SUBTRACT };
};


struct glCullFaceOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_FRONT", "GL_BACK", "GL_FRONT_AND_BACK" };
    inline const static std::vector<uint16_t> options = { GL_FRONT ,  GL_BACK ,  GL_FRONT_AND_BACK };
};

struct glCompOptions : public glOptions
{
    inline const static std::vector<std::string> names = { "GL_NEVER", "GL_LESS", "GL_LEQUAL", "GL_EQUAL", "GL_NOTEQUAL", "GL_GEQUAL", "GL_GREATER", "GL_ALWAYS" };
    inline const static std::vector<uint16_t> options = { GL_NEVER ,  GL_LESS ,  GL_LEQUAL ,  GL_EQUAL ,  GL_NOTEQUAL,   GL_GEQUAL ,  GL_GREATER ,  GL_ALWAYS };

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
    SelectedObjectReferences& selected_objects;

    QCheckBox* alphafunc_checkbox = new QCheckBox(this);
    QCheckBox* blendfunc_checkbox = new QCheckBox(this);
    QCheckBox* blendeq_checkbox = new QCheckBox(this);
    QCheckBox* faceculling_checkbox = new QCheckBox(this);
    QCheckBox* depthfunc_checkbox = new QCheckBox(this);
    QCheckBox* depthmask_checkbox = new QCheckBox(this);
    QCheckBox* colormask_checkbox = new QCheckBox(this);

    QComboBox* alphafunc_combobox = new QComboBox(this);
    QComboBox* blendfunc_src_combobox = new QComboBox(this);
    QComboBox* blendfunc_dst_combobox = new QComboBox(this);
    QComboBox* blendeq_combobox = new QComboBox(this);
    QComboBox* faceculling_combobox = new QComboBox(this);
    QComboBox* depthfunc_combobox = new QComboBox(this);
    QComboBox* depthmask_combobox = new QComboBox(this);
    QComboBox* colormask_combobox_r = new QComboBox(this);
    QComboBox* colormask_combobox_g = new QComboBox(this);
    QComboBox* colormask_combobox_b = new QComboBox(this);
    QComboBox* colormask_combobox_a = new QComboBox(this);


    QLineEdit* alphafunc_textbox = new QLineEdit(this);

    template<uint8_t setting_id, class FunctorT, int... active_ids>
    void handleCheckbox(int checkstate, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, QWidget* widget = Q_NULLPTR)
    {
        auto& material = this->selected_objects.getSelectedMaterial();
        if (!material)
            return;
        auto& settings = material->opengl_settings;
        auto setting_idx = this->checkIfSettingExists(setting_id, settings);

        // If ticked (or not ticked, depending on whether FunctorT is std::identity or std::logical_not)
        if (FunctorT()(checkstate))
        {
            // If the setting doesn't exist...
            if (setting_idx == -1)
            {
                material->addOpenGLSetting(setting_id, { v1, v2, v3, v4 });
            }
            if (widget)
                widget->setEnabled(true);
        }
        else
        {
            // If any of the dependencies exist, don't delete
            if constexpr (sizeof...(active_ids) > 0)
            {
                for (const auto& id : { active_ids... })
                {
                    auto id_idx = this->checkIfSettingExists(id, settings);
                    if (id_idx != -1)
                    {
                        return;
                    }
                }
            }

            // If no dependencies exist... check if it needs deleting
            if (setting_idx != -1)
            {
                settings.erase(settings.begin() + setting_idx);
            }
            if (widget)
                widget->setEnabled(false);
        }
    }

    template<typename F1, typename F2, typename F3, typename F4>
    void updateSettingData(uint8_t setting_id, F1 f1, F2 f2, F3 f3, F4 f4)
    {
        auto& material = this->selected_objects.getSelectedMaterial();
        if (!material)
            return;
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
    OpenGLSettingsWidget(SelectedObjectReferences& sor, QWidget* parent = Q_NULLPTR) 
        : QWidget(parent)
        , selected_objects(sor)
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
            [this, alphafunc_widget](int checkstate)
            {
                this->handleCheckbox<0xA0, identity>(checkstate, this->alphafunc_combobox->currentData().toInt(), 0, 0, 0, alphafunc_widget); // glAlphaFunc
                this->handleCheckbox<0xA1, identity>(checkstate, GL_TRUE, 0, 0, 0); // glEnable(GL_ALPHA_TEST)
            }
        );
        connect(this->alphafunc_combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->updateAlphaFunc(); });
        connect(this->alphafunc_textbox, &QLineEdit::textChanged, this, [this]() {this->updateAlphaFunc(); });
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
            [this, blendfunc_widget](int checkstate)
            {
                this->handleCheckbox<0xA2, identity>(checkstate, this->blendfunc_src_combobox->currentData().toInt(), this->blendfunc_dst_combobox->currentData().toInt(), 0, 0, blendfunc_widget); // BlendFunc GL_ONE, GL_ZERO
                this->handleCheckbox<0xA4, identity, 0xA2, 0xA3>(checkstate, GL_TRUE, 0, 0, 0); // glEnable(GL_BLEND) GL_TRUE
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
            [this, blendeq_widget](int checkstate)
            {
                this->handleCheckbox<0xA3, identity>(checkstate, this->blendeq_combobox->currentData().toInt(), 0, 0, 0, blendeq_widget); // BlendEquationSeparate GL_FUNC_ADD (check)
                this->handleCheckbox<0xA4, identity, 0xA2, 0xA3>(checkstate, GL_TRUE, 0, 0, 0); // glEnable(GL_BLEND)
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
            [this, faceculling_widget](int checkstate)
            {
                this->handleCheckbox<0xA5, identity>(checkstate, this->faceculling_combobox->currentData().toInt(), 0, 0, 0, faceculling_widget); // glCullFace
                this->handleCheckbox<0xA6, std::logical_not<decltype(checkstate)>, 0xA5>(checkstate, GL_TRUE, 0, 0, 0); // glDisable(GL_CULL_FACE)
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
            [this, depthfunc_widget](int checkstate)
            {
                this->handleCheckbox<0xA7, identity>(checkstate, this->depthfunc_combobox->currentData().toInt(), 0, 0, 0, depthfunc_widget); // glDepthFunc
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, 0xA7, 0xA8>(checkstate, GL_TRUE, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
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
            [this, depthmask_widget](int checkstate)
            {
                this->handleCheckbox<0xA8, identity>(checkstate, this->depthmask_combobox->currentData().toInt(), 0, 0, 0, depthmask_widget); // glDepthMask
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, 0xA7, 0xA8>(checkstate, GL_TRUE, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
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
            [this, colormask_widget](int checkstate)
            {
                this->handleCheckbox<0xAC, identity>(
                    checkstate,
                    this->colormask_combobox_r->currentData().toInt(),
                    this->colormask_combobox_g->currentData().toInt(),
                    this->colormask_combobox_b->currentData().toInt(),
                    this->colormask_combobox_a->currentData().toInt(),
                    colormask_widget
                    ); // glColorMask
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

        connect(&this->selected_objects, &SelectedObjectReferences::selectedMaterialUpdated, this, &OpenGLSettingsWidget::updateUI);
    }

    void updateUI()
    {
        auto& selected_material = this->selected_objects.getSelectedMaterial();

        if (int idx = this->checkIfSettingExists(0xA0, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glCompOptions::options.begin();
            auto end = glCompOptions::options.end();
            this->alphafunc_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->alphafunc_textbox->setText(QString::fromStdString(data[1]));
            this->alphafunc_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glCompOptions::options.begin();
            auto end = glCompOptions::options.end();
            this->alphafunc_combobox->setCurrentIndex(std::find(begin, end, GL_ALWAYS) - begin); // Should constexpr this
            this->alphafunc_textbox->setText("0.0");
            this->alphafunc_checkbox->setChecked(false);
            emit this->alphafunc_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xA2, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glBlendFuncOptions::options.begin();
            auto end = glBlendFuncOptions::options.end();
            this->blendfunc_src_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->blendfunc_dst_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[1])) - begin);
            this->blendfunc_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glBlendFuncOptions::options.begin();
            auto end = glBlendFuncOptions::options.end();
            this->blendfunc_src_combobox->setCurrentIndex(std::find(begin, end, GL_ONE) - begin); // Should constexpr this
            this->blendfunc_dst_combobox->setCurrentIndex(std::find(begin, end, GL_ZERO) - begin); // Should constexpr this
            this->blendfunc_checkbox->setChecked(false);
            emit this->blendfunc_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xA3, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glBlendEqOptions::options.begin();
            auto end = glBlendEqOptions::options.end();
            this->blendeq_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->blendeq_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glBlendEqOptions::options.begin();
            auto end = glBlendEqOptions::options.end();
            this->blendeq_combobox->setCurrentIndex(std::find(begin, end, GL_FUNC_ADD) - begin); // Should constexpr this
            this->blendeq_checkbox->setChecked(false);
            emit this->blendeq_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xA5, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glCullFaceOptions::options.begin();
            auto end = glCullFaceOptions::options.end();
            this->faceculling_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->faceculling_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glCullFaceOptions::options.begin();
            auto end = glCullFaceOptions::options.end();
            this->faceculling_combobox->setCurrentIndex(std::find(begin, end, GL_BACK) - begin); // Should constexpr this
            this->faceculling_checkbox->setChecked(false);
            emit this->faceculling_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xA7, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glCompOptions::options.begin();
            auto end = glCompOptions::options.end();
            this->depthfunc_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->depthfunc_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glCompOptions::options.begin();
            auto end = glCompOptions::options.end();
            this->depthfunc_combobox->setCurrentIndex(std::find(begin, end, GL_LESS) - begin); // Should constexpr this
            this->depthfunc_checkbox->setChecked(false);
            emit this->depthfunc_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xA8, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glBoolOptions::options.begin();
            auto end = glBoolOptions::options.end();
            this->depthmask_combobox->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->depthmask_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glBoolOptions::options.begin();
            auto end = glBoolOptions::options.end();
            this->depthmask_combobox->setCurrentIndex(std::find(begin, end, GL_TRUE) - begin); // Should constexpr this
            this->depthmask_checkbox->setChecked(false);
            emit this->depthmask_checkbox->stateChanged(0);
        }

        if (int idx = this->checkIfSettingExists(0xAC, selected_material->opengl_settings); idx != -1)
        {
            auto& setting = selected_material->opengl_settings[idx];
            auto data = setting->getData();

            auto begin = glBoolOptions::options.begin();
            auto end = glBoolOptions::options.end();
            this->colormask_combobox_r->setCurrentIndex(std::find(begin, end, std::stoi(data[0])) - begin);
            this->colormask_combobox_g->setCurrentIndex(std::find(begin, end, std::stoi(data[1])) - begin);
            this->colormask_combobox_b->setCurrentIndex(std::find(begin, end, std::stoi(data[2])) - begin);
            this->colormask_combobox_a->setCurrentIndex(std::find(begin, end, std::stoi(data[3])) - begin);
            this->colormask_checkbox->setChecked(true);
        }
        else
        {
            auto begin = glBoolOptions::options.begin();
            auto end = glBoolOptions::options.end();
            this->colormask_combobox_r->setCurrentIndex(std::find(begin, end, GL_TRUE) - begin); // Should constexpr this
            this->colormask_combobox_g->setCurrentIndex(std::find(begin, end, GL_TRUE) - begin); // Should constexpr this
            this->colormask_combobox_b->setCurrentIndex(std::find(begin, end, GL_TRUE) - begin); // Should constexpr this
            this->colormask_combobox_a->setCurrentIndex(std::find(begin, end, GL_TRUE) - begin); // Should constexpr this
            this->colormask_checkbox->setChecked(false);
            emit this->colormask_checkbox->stateChanged(0);
        }
    }
};