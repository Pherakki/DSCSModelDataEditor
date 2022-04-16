#include "OpenGLSettingsWidgets.hpp"

typedef std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial> MaterialPtr;
typedef std::vector<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLSettings::OpenGLSetting>> OGLS_t;


int8_t OpenGLSetterBase::checkIfSettingExists(uint8_t setting_id, const OGLS_t& settings)
{
    for (int8_t idx = 0; idx < settings.size(); ++idx)
    {
        if (settings[idx]->getID() == setting_id)
            return idx;
    }
    return -1;
}


std::string AlphaFuncSetting::sanitiseFloatText(const QString& value)
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


void AlphaFuncSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox->currentData().toString().toStdString(); },
        [this]() { return this->sanitiseFloatText(this->textbox->text()); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}


void BlendFuncSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->src_combobox->currentData().toString().toStdString(); },
        [this]() { return this->dst_combobox->currentData().toString().toStdString(); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}


void BlendEqSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox->currentData().toString().toStdString(); },
        []() { return std::string(""); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}


void CullFaceSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox->currentData().toString().toStdString(); },
        []() { return std::string(""); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}


void DepthFuncSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox->currentData().toString().toStdString(); },
        []() { return std::string(""); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}


void DepthMaskSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox->currentData().toString().toStdString(); },
        []() { return std::string(""); },
        []() { return std::string(""); },
        []() { return std::string(""); }
    );
}



void ColorMaskSetting::update()
{
    this->updateSettingData
    (
        [this]() { return this->combobox_r->currentData().toString().toStdString(); },
        [this]() { return this->combobox_g->currentData().toString().toStdString(); },
        [this]() { return this->combobox_b->currentData().toString().toStdString(); },
        [this]() { return this->combobox_a->currentData().toString().toStdString(); }
    );
}