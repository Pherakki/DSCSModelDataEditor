#pragma once

#include <unordered_map>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
// #include<Rendering/ShaderBackends/AbstractShaderBackend.hpp>
#include<Rendering/ShaderBackends/cgGL/cgGLShaderObject.hpp>
#include <debugapi.h>
#include<sstream>

namespace Rendering::ShaderBackends
{
	// Should make this polymorphic eventually, but too much hassle for now
	class cgGLShaderBackend// : public AbstractShaderBackend
	{
	private:
		CGcontext context;
		CGprofile vertex_profile;
		CGprofile fragment_profile;

		CGprogram compileShader(const std::string& shader_src, const CGprofile& profile, const std::string& entry_point);
	public:
		cgGLShaderBackend();
		~cgGLShaderBackend();// override;
		std::shared_ptr<ShaderObjects::cgGLShaderObject> createShaderProgram(const std::string& vertex_shader_path,
			const std::string& fragment_shader_path);// override;
		const void checkForCgError(const std::string& message);
	};

	// Constructor
	cgGLShaderBackend::cgGLShaderBackend()
	{
		this->context = cgCreateContext();
		vertex_profile = CG_PROFILE_ARBVP1;
		fragment_profile = CG_PROFILE_ARBFP1;
		// I think some OpenGL extensions are required for these to work
		//vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
		//cgGLSetContextOptimalOptions(this->context, vertex_profile);
		//fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		//cgGLSetContextOptimalOptions(this->context, fragment_profile);
	}

	// Destructor
	cgGLShaderBackend::~cgGLShaderBackend()
	{	
		cgDestroyContext(this->context);
	}


	// createShaderProgram
	std::shared_ptr<ShaderObjects::cgGLShaderObject> cgGLShaderBackend::createShaderProgram(const std::string& vertex_shader_path,
	                                                                       const std::string& fragment_shader_path)
	{
		CGprogram vertex_program = cgGLShaderBackend::compileShader(vertex_shader_path, this->vertex_profile, "std_VS");
		this->checkForCgError("Compiling vertex program...");
		CGprogram fragment_program = cgGLShaderBackend::compileShader(fragment_shader_path, this->fragment_profile, "std_PS");
		this->checkForCgError("Compiling fragment program...");

		this->checkForCgError("Combining programs...");
		std::shared_ptr<ShaderObjects::cgGLShaderObject> shader_object = std::make_shared<ShaderObjects::cgGLShaderObject>(vertex_program, this->vertex_profile,
			                                                                                                               fragment_program, this->fragment_profile);

		shader_object->load();
		this->checkForCgError("Loading program...");

		cgDestroyProgram(vertex_program);
		cgGLDisableProfile(this->vertex_profile);
		cgDestroyProgram(fragment_program);
		cgGLDisableProfile(this->fragment_profile);
		this->checkForCgError("Finalising...");

		shader_object->unload();
		return shader_object;
	}

	// Might be able to get the entry point from the code I guess, but let's stick with this unless it becomes a problem...
	// compileShader
	inline CGprogram cgGLShaderBackend::compileShader(const std::string& filepath, const CGprofile& profile, const std::string& entry_point)
	{
		cgGLEnableProfile(profile);
		return cgCreateProgramFromFile(this->context, CG_SOURCE, filepath.c_str(), profile, entry_point.c_str(), NULL);
	}

	// checkForCgError
	const void cgGLShaderBackend::checkForCgError(const std::string& message)
	{
		CGerror error;
		const char* stringout = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR)
		{
			std::stringstream ss;
			ss << "ERROR: " << message << ' ' << stringout << '\n' << std::endl;
			OutputDebugStringA(ss.str().c_str());
			if (error == CG_COMPILER_ERROR)
			{
				std::stringstream ss;
				ss << cgGetLastListing(this->context) << '\n' << std::endl;
				OutputDebugStringA(ss.str().c_str());
			}
			exit(1);
		}
	}

}