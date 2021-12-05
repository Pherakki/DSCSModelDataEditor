#pragma once

#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

#include "../../../../../Cg/cg.h"
#include "../../../../../Cg/cgGL.h"

#include "ShaderVariables.hpp"
#include "Utils.hpp"


namespace Rendering::ShaderObjects
{
	typedef std::unordered_map<uint8_t, CGparameter> cgParamMap;

	class cgGLShaderObject
	{
	public:

		cgGLShaderObject(const std::string& vertex_source, CGprofile vertex_profile,
			             const std::string& fragment_source, CGprofile fragment_profile,
			             const CGcontext& context);
		~cgGLShaderObject();
		void load();
		void unload();
		void bind();
		void unbind();
		CGprogram compileShader(const CGcontext& context, const std::string& src, const CGprofile& profile, const std::string& entry_point);
		void setVertexShader(const std::string& vertex_source, const CGcontext& context);
		void setFragmentShader(const std::string& fragment_source, const CGcontext& context);
		void getVertexParametersFromProgram(cgParamMap& uniforms);
		void getVertexParametersFromProgram(cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs);
		void getFragmentParametersFromProgram(cgParamMap& uniforms);
		void getFragmentParametersFromProgram(cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs);

	//private:
		std::string vertex_source;
		std::string fragment_source;
		CGprofile vertex_profile;
		CGprofile fragment_profile;
		CGprogram vertex_program;
		CGprogram fragment_program;

		void getParametersFromProgram(const CGprogram& program, cgParamMap& uniforms);
		void getParametersFromProgram(const CGprogram& program, cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs);

	};

}
