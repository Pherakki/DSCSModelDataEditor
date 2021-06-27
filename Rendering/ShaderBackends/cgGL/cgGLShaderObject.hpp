#pragma once

#include <Cg/cg.h>
#include<Rendering/ShaderBackends/AbstractShaderObject.hpp>

namespace Rendering::ShaderObjects
{
	class cgGLShaderObject// : public AbstractShaderObject
	{
	public:
		cgGLShaderObject(const CGprogram &vertex_program, CGprofile vertex_profile, 
			             const CGprogram& fragment_program, CGprofile fragment_profile);
		~cgGLShaderObject();// override;
		void load();
		void unload();
		void bind();
		void unbind();
		void setMVP(const std::array<float, 16>&);
	private:
		CGprofile vertex_profile;
		CGprofile fragment_profile;
		CGprogram program;
		std::vector<std::string> vertex_parameters;
		std::vector<std::string> fragment_parameters;

		std::vector<std::string> getParameterNamesFromProgram(CGprogram);
	};

	// Constructor
	cgGLShaderObject::cgGLShaderObject(const CGprogram& vertex_program, CGprofile vertex_profile,
	                                   const CGprogram& fragment_program, CGprofile fragment_profile)
	{
		this->vertex_profile = vertex_profile;
		this->fragment_profile = fragment_profile;
		this->program = cgCombinePrograms2(vertex_program, fragment_program);
		
		this->vertex_parameters = this->getParameterNamesFromProgram(vertex_program);
		this->fragment_parameters = this->getParameterNamesFromProgram(fragment_program);
	}

	// Destructor
	cgGLShaderObject::~cgGLShaderObject()
	{
		cgDestroyProgram(this->program);
	}

	// getParameterNamesFromProgram
	std::vector<std::string> cgGLShaderObject::getParameterNamesFromProgram(CGprogram program)
	{
		std::vector<std::string> out;
		CGparameter param = cgGetFirstParameter(program, CG_PROGRAM);
		while (param)
		{
			std::string param_name = cgGetParameterName(param);
			out.push_back(param_name);
			param = cgGetNextParameter(param);
		}
		return out;
	}

	void cgGLShaderObject::load()
	{
		cgGLLoadProgram(this->program);
	}

	void cgGLShaderObject::unload()
	{
		cgGLUnloadProgram(this->program);
	}

	void cgGLShaderObject::bind()
	{
		cgGLEnableProfile(this->vertex_profile);
		cgGLEnableProfile(this->fragment_profile);
		cgGLLoadProgram(this->program);
		cgGLBindProgram(this->program);
	}

	void cgGLShaderObject::unbind()
	{
		cgGLUnloadProgram(this->program);
		cgGLDisableProfile(this->vertex_profile);
		cgGLDisableProfile(this->fragment_profile);
	}

	void cgGLShaderObject::setMVP(const std::array<float, 16>& mvp)
	{
		CGparameter mvp_parameter = cgGetNamedParameter(this->program, "ModelViewProjection");
		cgGLSetMatrixParameterfc(mvp_parameter, &mvp.front());

	}

}
