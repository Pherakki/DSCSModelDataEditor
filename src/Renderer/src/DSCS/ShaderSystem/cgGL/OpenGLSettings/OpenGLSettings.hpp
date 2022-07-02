#pragma once

#include <array>
#include <functional>
#include <string>

namespace Rendering::DSCS::DataObjects::OpenGLSettings
{
	enum class glFlagTypes
	{
		Null,
		Float,
		glBool,
		glComparison,
		glBlendFunc,
		glCullFace,
		glBlendEquationSeparate
	};


	class OpenGLSetting
	{
		// Should hold a lookup key and a payload
	private:
		uint8_t id;
	public:
		OpenGLSetting(uint8_t id) { this->id = id; };
		//virtual ~OpenGLSetting() = 0;
		virtual void set() {};
		virtual void unset() {};
		uint8_t getID() { return this->id; }
		virtual std::array<glFlagTypes, 4> getFlags() { return { glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		virtual std::array<std::string, 4> getData()  { return { "", "", "", "" }; }
		virtual void                       setData(const std::array<std::string, 4>&) = 0;

	};

	// ############################### //
	//            GLENABLE             //
	// ############################### //
	class OpenGLSettingGlEnable : public OpenGLSetting
	{
	public:
		OpenGLSettingGlEnable(uint8_t id, uint16_t gl_flag, bool isEnabled, bool default_val) : OpenGLSetting(id) { this->gl_flag = gl_flag; this->default_val = default_val; setEnabledState(isEnabled); };
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glBool, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->isEnabled), "", "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->isEnabled = std::stoi(data[0]); }
	private:
		uint16_t gl_flag;
		bool isEnabled;
		bool default_val;

		std::function<void(void)> setFunc;
		std::function<void(void)> unsetFunc;
		inline void enable_flag();
		inline void disable_flag();
		void setEnabledState(bool isEnabled);
	};


	// ############################### //
	//           GLALPHAFUNC           //
	// ############################### //
	class OpenGLSettingGlAlphaFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlAlphaFunc(uint8_t id, uint16_t func, float ref) : OpenGLSetting(id) { this->func = func, this->ref = ref; };
		uint16_t func;
		float ref;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glComparison, glFlagTypes::Float, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->func), std::to_string(this->ref), "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->func = std::stoi(data[0]); this->ref = std::stof(data[1]); }
	};



	// ############################### //
	//           GLBLENDFUNC           //
	// ############################### //
	class OpenGLSettingGlBlendFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlBlendFunc(uint8_t id, uint16_t src_blend_factor, uint16_t dst_blend_factor) : OpenGLSetting(id) { this->src_blend_factor = src_blend_factor, this->dst_blend_factor = dst_blend_factor; };
		uint16_t src_blend_factor;
		uint16_t dst_blend_factor;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glBlendFunc, glFlagTypes::glBlendFunc, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->src_blend_factor), std::to_string(this->dst_blend_factor), "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->src_blend_factor = std::stoi(data[0]); this->dst_blend_factor = std::stoi(data[1]); }
	};

	// ############################### //
	//     GLBLENDEQUATIONSEPARATE     //
	// ############################### //
	class OpenGLSettingGlBlendEquationSeparate : public OpenGLSetting
	{
	public:
		OpenGLSettingGlBlendEquationSeparate(uint8_t id, uint16_t rgba_blend_factor) : OpenGLSetting(id) { this->rgba_blend_factor = rgba_blend_factor; };
		uint16_t rgba_blend_factor;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glBlendEquationSeparate, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->rgba_blend_factor), "", "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->rgba_blend_factor = std::stoi(data[0]); }
	};


	// ############################### //
	//            GLCULLFACE           //
	// ############################### //
	class OpenGLSettingGlCullFace : public OpenGLSetting
	{
	public:
		OpenGLSettingGlCullFace(uint8_t id, uint16_t mode) : OpenGLSetting(id) { this->mode = mode; };
		uint16_t mode;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glCullFace, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(mode), "", "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->mode = std::stoi(data[0]); }
	};


	// ############################### //
	//           GLDEPTHFUNC           //
	// ############################### //
	class OpenGLSettingGlDepthFunc : public OpenGLSetting
	{
	public:
		OpenGLSettingGlDepthFunc(uint8_t id, uint16_t func) : OpenGLSetting(id) { this->func = func; };
		uint16_t func;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glComparison, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->func), "", "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->func = std::stoi(data[0]); }
	};


	// ############################### //
	//           GLDEPTHMASK           //
	// ############################### //
	class OpenGLSettingGlDepthMask : public OpenGLSetting
	{
	public:
		OpenGLSettingGlDepthMask(uint8_t id, bool gl_bool) : OpenGLSetting(id) { this->gl_bool = gl_bool; };
		bool gl_bool;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glBool, glFlagTypes::Null, glFlagTypes::Null, glFlagTypes::Null }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->gl_bool), "", "", "" }; }
		void setData(const std::array<std::string, 4>& data) override { this->gl_bool = std::stoi(data[0]); }
	};


	// ############################### //
	//           GLCOLORMASK           //
	// ############################### //
	class OpenGLSettingGlColorMask : public OpenGLSetting
	{
	public:
		OpenGLSettingGlColorMask(uint8_t id, bool r, bool g, bool b, bool a) : OpenGLSetting(id) { this->r = r; this->g = g; this->b = b; this->a = a; };
		bool r;
		bool g;
		bool b;
		bool a;
		void set() override;
		void unset() override;
		std::array<glFlagTypes, 4> getFlags() override { return { glFlagTypes::glBool, glFlagTypes::glBool, glFlagTypes::glBool, glFlagTypes::glBool }; }
		std::array<std::string, 4> getData()  override { return { std::to_string(this->r), std::to_string(this->g), std::to_string(this->b) , std::to_string(this->a) }; }
		void setData(const std::array<std::string, 4>& data) override { this->r = std::stoi(data[0]); this->g = std::stoi(data[1]); this->b = std::stoi(data[2]); this->a = std::stoi(data[3]); }
	};
}