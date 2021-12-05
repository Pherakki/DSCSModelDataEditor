#pragma once
#include <QOpenGLFunctions_2_1>
#include <functional>

namespace Rendering::DSCS::DataObjects::OpenGLSettings
{
	class OpenGLSetting : protected QOpenGLFunctions_2_1
	{
		// Should hold a lookup key and a payload
	public:
		OpenGLSetting() { initializeOpenGLFunctions(); };
		//virtual ~OpenGLSetting() = 0;
		virtual void set() {};
		virtual void unset() {};
	};

	// ############################### //
	//            GLENABLE             //
	// ############################### //
	class OpenGLSettingGlEnable : public OpenGLSetting
	{
	public:
		OpenGLSettingGlEnable(uint16_t gl_flag, bool isEnabled, bool default_val) : OpenGLSetting() { this->gl_flag = gl_flag; setEnabledState(isEnabled); this->default_val = default_val; };
		void set() override   { this->setFunc(); };
		void unset() override { };
	private:
		uint16_t gl_flag;
		bool isEnabled;
		bool default_val;

		std::function<void(void)> setFunc;
		std::function<void(void)> unsetFunc;
		inline void enable_flag()  { glEnable(this->gl_flag); };
		inline void disable_flag() { glDisable(this->gl_flag); };
		void setEnabledState(bool isEnabled)
		{
			if (isEnabled)
			{
				this->setFunc = [this]() {this->OpenGLSettingGlEnable::enable_flag(); };
			}
			else
			{
				this->setFunc = [this]() {this->OpenGLSettingGlEnable::disable_flag(); };
			}
			if (default_val)
				this->unsetFunc = [this]() {this->OpenGLSettingGlEnable::enable_flag(); };
			else
				this->unsetFunc = [this]() {this->OpenGLSettingGlEnable::disable_flag(); };
			this->isEnabled = isEnabled;
		};
	};


	// ############################### //
	//           GLALPHAFUNC           //
	// ############################### //
	class OpenGLSettingGlAlphaFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlAlphaFunc(uint16_t func, float ref) : OpenGLSetting() { this->func = func, this->ref = ref; };
		uint16_t func;
		float ref;
		void set() override   { glAlphaFunc(this->func, this->ref); };
		void unset() override { };
	};



	// ############################### //
	//           GLBLENDFUNC           //
	// ############################### //
	class OpenGLSettingGlBlendFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlBlendFunc(uint16_t src_blend_factor, uint16_t dst_blend_factor) : OpenGLSetting() { this->src_blend_factor = src_blend_factor, this->dst_blend_factor = dst_blend_factor; };
		uint16_t src_blend_factor;
		uint16_t dst_blend_factor;
		void set() override   { glBlendFunc(this->src_blend_factor, this->dst_blend_factor); };
		void unset() override { glBlendFunc(GL_ONE, GL_ZERO); };
	};


	// ############################### //
	//     GLBLENDEQUATIONSEPARATE     //
	// ############################### //
	class OpenGLSettingGlBlendEquationSeparate : public OpenGLSetting
	{
	public:
		OpenGLSettingGlBlendEquationSeparate(uint16_t rgba_blend_factor) : OpenGLSetting() { this->rgba_blend_factor = rgba_blend_factor; };
		uint16_t rgba_blend_factor;
		void set() override   { glBlendEquationSeparate(this->rgba_blend_factor, this->rgba_blend_factor); };
		void unset() override { };
	};


	// ############################### //
	//            GLCULLFACE           //
	// ############################### //
	class OpenGLSettingGlCullFace : public OpenGLSetting
	{
	public:
		OpenGLSettingGlCullFace(uint16_t mode) : OpenGLSetting() { this->mode = mode; };
		uint16_t mode;
		void set() override   { glCullFace(this->mode); };
		void unset() override { glCullFace(GL_BACK); };
	};



	// ############################### //
	//           GLDEPTHFUNC           //
	// ############################### //
	class OpenGLSettingGlDepthFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlDepthFunc(uint16_t func) : OpenGLSetting() { this->func = func; };
		uint16_t func;
		void set() override   { glDepthFunc(this->func); };
		void unset() override { glDepthFunc(GL_LESS); };
	};


	// ############################### //
	//           GLDEPTHMASK           //
	// ############################### //
	class OpenGLSettingGlDepthMask : public OpenGLSetting
	{
	public:
		OpenGLSettingGlDepthMask(bool gl_bool) : OpenGLSetting() { this->gl_bool = gl_bool; };
		bool gl_bool;
		void set() override   { glDepthMask(this->gl_bool); };
		void unset() override { glDepthMask(true); };
	};


	// ############################### //
	//           GLCOLORMASK           //
	// ############################### //
	class OpenGLSettingGlColorMask : public OpenGLSetting
	{
	public:
		OpenGLSettingGlColorMask(bool r, bool g, bool b, bool a) : OpenGLSetting() { this->r = r; this->g = g; this->b = b; this->a = a; };
		bool r;
		bool g;
		bool b;
		bool a;
		void set() override   { glColorMask(this->r, this->g, this->b, this->a); };
		void unset() override { glColorMask(1, 1, 1, 1); };
	};


}