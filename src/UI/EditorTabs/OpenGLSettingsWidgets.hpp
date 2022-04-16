#pragma once

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include "../Types.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"


/// #########################
///       OPENGL OPTIONS
/// #########################

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

/// #########################
///      OPENGLSETTERBASE
/// #########################
/// 
class OpenGLSetterBase : public QWidget
{
private: 
    typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;
    typedef std::vector<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLSettings::OpenGLSetting>> OGLS_t;

    QLabel*      label        = new QLabel(this);
    QWidget*     values       = new QWidget(this);
    MaterialPtr selected_material = nullptr;

protected:
    inline const static uint8_t id = NULL;
    inline const static QString name = "";

    QCheckBox* checkbox = new QCheckBox(this);
    QHBoxLayout* value_layout = new QHBoxLayout;

    template <typename option_pack>
    requires std::is_base_of<glOptions, option_pack>::value
    void initComboBox(QComboBox* combobox)
    {
        for (size_t i = 0; i < option_pack::names.size(); ++i)
        {
            combobox->addItem(QString::fromStdString(option_pack::names[i]), option_pack::options[i]);
        }
    }

    void hookTextBox(QLineEdit* textbox)
    {
        connect(textbox, &QLineEdit::textChanged, this, [this]() {this->update(); });
    }

    void hookComboBox(QComboBox* combobox)
    {
        connect(combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() {this->update(); });
    }


public:
    OpenGLSetterBase(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : QWidget(parent)
    {
        parent_layout->addWidget(this->checkbox, row, 0);
        parent_layout->addWidget(this->label, row, 1);
        parent_layout->addWidget(this->values, row, 2);
        this->values->setLayout(this->value_layout);
        this->label->setText(this->name);
        ++row;
    }

    template<uint8_t setting_id, class FunctorT, int... active_ids>
    void handleCheckbox(int checkstate, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);

    template<typename F1, typename F2, typename F3, typename F4>
    void updateSettingData(F1 f1, F2 f2, F3 f3, F4 f4);

    int8_t checkIfSettingExists(uint8_t setting_id, const OGLS_t& settings);

    virtual void update() = 0;

    void updateSelectedMaterial(MaterialPtr material_ptr)
    {
        this->selected_material = material_ptr;
    }
};

template<uint8_t setting_id, class FunctorT, int ...active_ids>
void OpenGLSetterBase::handleCheckbox(int checkstate, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4)
{
    auto& material = this->selected_material;
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
    }
    else
    {
        // If any of the dependencies exist, don't delete
        if constexpr (sizeof...(active_ids))
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
    }
}

template<typename F1, typename F2, typename F3, typename F4>
void OpenGLSetterBase::updateSettingData(F1 f1, F2 f2, F3 f3, F4 f4)
{
    auto& material = this->selected_material;
    if (!material)
        return;
    auto& settings = material->opengl_settings;
    auto setting_idx = this->checkIfSettingExists(this->id, settings);
    if (setting_idx != -1)
    {
        auto& setting = material->opengl_settings[setting_idx];
        setting->setData({ f1(), f2(), f3(), f4() });
    }
}


/// #########################
///  DERIVED SETTING CLASSES
/// #########################

class AlphaFuncSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA0;
    inline const static std::string name = "Alpha Function";
    QComboBox* combobox = new QComboBox(this);
    QLineEdit* textbox = new QLineEdit(this);
    std::string sanitiseFloatText(const QString& value);
public:
    AlphaFuncSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent=Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    { 
        // UI
        this->value_layout->addWidget(this->combobox); 
        this->value_layout->addWidget(this->textbox); 
        // Inits
        this->initComboBox<glCompOptions>(this->combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_NEVER, 0, 0, 0); // glAlphaFunc
                this->handleCheckbox<0xA1, std::identity>(checkstate, GL_TRUE, 0, 0, 0); // glEnable(GL_ALPHA_TEST)
            }
        );
        this->hookComboBox(this->combobox);
        this->hookTextBox(this->textbox);

    }
    void update() override;
};


class BlendFuncSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA2;
    inline const static std::string name = "Blend Function";
    QComboBox* src_combobox = new QComboBox(this);
    QComboBox* dst_combobox = new QComboBox(this);
public:
    BlendFuncSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->src_combobox); 
        this->value_layout->addWidget(this->dst_combobox);
        // Inits
        this->initComboBox<glBlendFuncOptions>(this->src_combobox);
        this->initComboBox<glBlendFuncOptions>(this->dst_combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_ONE, GL_ZERO, 0, 0); // BlendFunc
                this->handleCheckbox<0xA4, std::identity, this->id, 0xA3>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        this->hookComboBox(this->src_combobox);
        this->hookComboBox(this->dst_combobox);
    }
    void update() override;
};


class BlendEqSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA3;
    inline const static std::string name = "Blend Equation";
    QComboBox* combobox = new QComboBox(this);
public:
    BlendEqSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->combobox); 
        // Inits
        this->initComboBox<glBlendEqOptions>(this->combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_FUNC_ADD, 0, 0, 0); // BlendEquationSeparate
                this->handleCheckbox<0xA4, std::identity, 0xA2, this->id>(checkstate, 1, 0, 0, 0); // glEnable(GL_BLEND)
            }
        );
        this->hookComboBox(this->combobox);
    }
    void update() override;
};


class CullFaceSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA5;
    inline const static std::string name = "Face Culling";
    QComboBox* combobox = new QComboBox(this);
public:
    CullFaceSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->combobox); 
        // Inits
        this->initComboBox<glCullFaceOptions>(this->combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_BACK, 0, 0, 0); // glCullFace
            }
        );
        this->hookComboBox(this->combobox);
    }
    void update() override;
};


class DepthFuncSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA7;
    inline const static std::string name = "Depth Function";
    QComboBox* combobox = new QComboBox(this);
public:
    DepthFuncSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->combobox); 
        // Inits
        this->initComboBox<glCompOptions>(this->combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_LESS, 0, 0, 0); // glDepthFunc
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, this->id, 0xA8>(checkstate, 1, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
            }
        );
        this->hookComboBox(this->combobox);
    }
    void update() override;
};


class DepthMaskSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xA8;
    inline const static std::string name = "Depth Mask";
    QComboBox* combobox = new QComboBox(this);
public:
    DepthMaskSetting(size_t& row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->combobox);
        // Inits
        this->initComboBox<glBoolOptions>(this->combobox);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_TRUE, 0, 0, 0); // glDepthMask
                this->handleCheckbox<0xA9, std::logical_not<decltype(checkstate)>, 0xA7, this->id>(checkstate, 1, 0, 0, 0); // glDisable(GL_DEPTH_TEST)
            }
        );
        this->hookComboBox(this->combobox);
    }
    void update() override;
};


class ColorMaskSetting : public OpenGLSetterBase
{
protected:
    inline const static uint8_t id = 0xAC;
    inline const static std::string name = "Color Mask";
    QComboBox* combobox_r = new QComboBox(this);
    QComboBox* combobox_g = new QComboBox(this);
    QComboBox* combobox_b = new QComboBox(this);
    QComboBox* combobox_a = new QComboBox(this);
public:
    ColorMaskSetting(size_t row, QGridLayout* parent_layout, QWidget* parent = Q_NULLPTR) : OpenGLSetterBase(row, parent_layout, parent)
    {
        // UI
        this->value_layout->addWidget(this->combobox_r);
        this->value_layout->addWidget(this->combobox_g);
        this->value_layout->addWidget(this->combobox_b);
        this->value_layout->addWidget(this->combobox_a);
        // Inits
        this->initComboBox<glBoolOptions>(this->combobox_r);
        this->initComboBox<glBoolOptions>(this->combobox_g);
        this->initComboBox<glBoolOptions>(this->combobox_b);
        this->initComboBox<glBoolOptions>(this->combobox_a);
        // Connections
        connect(this->checkbox, &QCheckBox::stateChanged, this,
            [this](int checkstate)
            {
                this->handleCheckbox<this->id, std::identity>(checkstate, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // glColorMask
            }
        );
        this->hookComboBox(this->combobox_r);
        this->hookComboBox(this->combobox_g);
        this->hookComboBox(this->combobox_b);
        this->hookComboBox(this->combobox_a);
    }
    void update() override;
};
