#include "cgGLShaderObject.hpp"

namespace Rendering::ShaderObjects
{

	// Constructor
	cgGLShaderObject::cgGLShaderObject(const std::string& vertex_source, CGprofile vertex_profile,
		const std::string& fragment_source, CGprofile fragment_profile,
		const CGcontext& context)
	{
		this->vertex_profile = vertex_profile;
		this->fragment_profile = fragment_profile;
		this->setVertexShader(vertex_source, context);
		this->setFragmentShader(fragment_source, context);

	}

	// Destructor
	cgGLShaderObject::~cgGLShaderObject()
	{
		this->unbind();
		this->unload();
		cgDestroyProgram(this->vertex_program);
		cgDestroyProgram(this->fragment_program);
	}

	void cgGLShaderObject::setVertexShader(const std::string& vertex_source, const CGcontext& context)
	{
		this->vertex_source = vertex_source;
		cgGLEnableProfile(this->vertex_profile);
		std::string entry_point = "std_VS";
		this->vertex_program = cgCreateProgram(context, CG_SOURCE, vertex_source.c_str(), this->vertex_profile, entry_point.c_str(), NULL);
		checkForCgError(context, "Compiling vertex shader...");
	}

	void cgGLShaderObject::setFragmentShader(const std::string& fragment_source, const CGcontext& context)
	{
		this->fragment_source = fragment_source;

		cgGLEnableProfile(this->fragment_profile);
		std::string entry_point = "std_PS";
		this->fragment_program = cgCreateProgram(context, CG_SOURCE, fragment_source.c_str(), this->fragment_profile, entry_point.c_str(), NULL);
		checkForCgError(context, "Compiling fragment shader...");

	}

	// getParameterNamesFromProgram
	void cgGLShaderObject::getParametersFromProgram(const CGprogram& program, cgParamMap& uniforms)
	{
		CGparameter param = cgGetFirstParameter(program, CG_PROGRAM);
		while (param)
		{
			const std::string param_name = cgGetParameterName(param);
			const CGenum param_variability = cgGetParameterVariability(param);
			const CGenum param_direction = cgGetParameterDirection(param);
			if (param_variability == CG_UNIFORM)
			{
				const uint8_t param_id = uniform_name_to_id.at(param_name);
				uniforms[param_id] = param;
			}
			param = cgGetNextParameter(param);
		}
	}
	void cgGLShaderObject::getParametersFromProgram(const CGprogram& program, cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs)
	{
		CGparameter param = cgGetFirstParameter(program, CG_PROGRAM);
		while (param)
		{
			const std::string param_name = cgGetParameterName(param);
			const CGenum param_variability = cgGetParameterVariability(param);
			const CGenum param_direction = cgGetParameterDirection(param);
			const uint8_t param_id = uniform_name_to_id.at(param_name);
			if (param_variability == CG_UNIFORM)
				uniforms[param_id] = param;
			else if (param_variability == CG_VARYING && param_direction == CG_IN)
				ins[param_id] = param;
			else if (param_variability == CG_VARYING && param_direction == CG_OUT)
				outs[param_id] = param;
			else
				std::exception("Unhandled parameter encountered.");
			param = cgGetNextParameter(param);
		}
	}

	// getVertexParametersFromProgram
	void cgGLShaderObject::getVertexParametersFromProgram(cgParamMap& uniforms)
	{
		this->getParametersFromProgram(this->vertex_program, uniforms);
	}
	void cgGLShaderObject::getVertexParametersFromProgram(cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs)
	{
		this->getParametersFromProgram(this->vertex_program, ins, uniforms, outs);
	}

	// getFragmentParametersFromProgram
	void cgGLShaderObject::getFragmentParametersFromProgram(cgParamMap& uniforms)
	{
		this->getParametersFromProgram(this->fragment_program, uniforms);
	}
	void cgGLShaderObject::getFragmentParametersFromProgram(cgParamMap& ins, cgParamMap& uniforms, cgParamMap& outs)
	{
		this->getParametersFromProgram(this->fragment_program, ins, uniforms, outs);
	}



	void cgGLShaderObject::load()
	{
		cgGLLoadProgram(this->vertex_program);
		cgGLLoadProgram(this->fragment_program);
	}

	void cgGLShaderObject::unload()
	{
		cgGLUnloadProgram(this->vertex_program);
		cgGLUnloadProgram(this->fragment_program);
	}

	void cgGLShaderObject::bind()
	{
		cgGLEnableProfile(this->vertex_profile);
		cgGLEnableProfile(this->fragment_profile);
		cgGLBindProgram(this->vertex_program);
		cgGLBindProgram(this->fragment_program);

	}

	void cgGLShaderObject::unbind()
	{
		// Note that UnbindProgram requires a profile to be passed as the parameter, since Cg unbinds the shader assigned to that profile slot
		cgGLUnbindProgram(this->vertex_profile);
		cgGLUnbindProgram(this->fragment_profile);
		cgGLDisableProfile(this->vertex_profile);
		cgGLDisableProfile(this->fragment_profile);
	}

	inline CGprogram cgGLShaderObject::compileShader(const CGcontext& context, const std::string& src, const CGprofile& profile, const std::string& entry_point)
	{
		cgGLEnableProfile(profile);
		return cgCreateProgram(context, CG_SOURCE, src.c_str(), profile, entry_point.c_str(), NULL);
	}
}