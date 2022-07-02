#include "OpenGLSettings.hpp"

#include "../../../../../libs/glad/include/glad/glad.h"

namespace Rendering::DSCS::DataObjects::OpenGLSettings
{
	// OpenGLSettingGlEnable

	void OpenGLSettingGlEnable::set()   { this->setFunc(); };
	void OpenGLSettingGlEnable::unset() { this->unsetFunc(); };
	void OpenGLSettingGlEnable::enable_flag()  { glEnable(this->gl_flag); }
	void OpenGLSettingGlEnable::disable_flag() { glDisable(this->gl_flag); }
	void OpenGLSettingGlEnable::setEnabledState(bool isEnabled)
	{
		if (isEnabled)
		{
			this->setFunc = [this]() {this->OpenGLSettingGlEnable::enable_flag(); };
		}
		else
		{
			this->setFunc = [this]() {this->OpenGLSettingGlEnable::disable_flag(); };
		}
		if (this->default_val)
			this->unsetFunc = [this]() {this->OpenGLSettingGlEnable::enable_flag(); };
		else
			this->unsetFunc = [this]() {this->OpenGLSettingGlEnable::disable_flag(); };
		this->isEnabled = isEnabled;
	};

	// OpenGLSettingGlAlphaFunc

	void OpenGLSettingGlAlphaFunc::set()   { glAlphaFunc(this->func, this->ref); };
	void OpenGLSettingGlAlphaFunc::unset() { };

	// OpenGLSettingGlBlendFunc

	void OpenGLSettingGlBlendFunc::set()   { glBlendFunc(this->src_blend_factor, this->dst_blend_factor); };
	void OpenGLSettingGlBlendFunc::unset() { glBlendFunc(GL_ONE, GL_ZERO); };

	// OpenGLSettingGlBlendEquationSeparate

	void OpenGLSettingGlBlendEquationSeparate::set()   { glBlendEquationSeparate(this->rgba_blend_factor, this->rgba_blend_factor); }
	void OpenGLSettingGlBlendEquationSeparate::unset() { };

	// OpenGLSettingGlCullFace

	void OpenGLSettingGlCullFace::set()   { glCullFace(this->mode); };
	void OpenGLSettingGlCullFace::unset() { glCullFace(GL_BACK); };

	// OpenGLSettingGlDepthFunc

	void OpenGLSettingGlDepthFunc::set()   { glDepthFunc(this->func); };
	void OpenGLSettingGlDepthFunc::unset() { glDepthFunc(GL_LESS); };

	// OpenGLSettingGlDepthMask

	void OpenGLSettingGlDepthMask::set()   { glDepthMask(this->gl_bool); };
	void OpenGLSettingGlDepthMask::unset() { glDepthMask(true); };

	// OpenGLSettingGlColorMask

	void OpenGLSettingGlColorMask::set()   { glColorMask(this->r, this->g, this->b, this->a); };
	void OpenGLSettingGlColorMask::unset() { glColorMask(1, 1, 1, 1); };
}