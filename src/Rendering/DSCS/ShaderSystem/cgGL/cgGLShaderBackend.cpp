#include "cgGLShaderBackend.hpp"

namespace Rendering::ShaderBackends
{

	// Constructor
	cgGLShaderBackend::cgGLShaderBackend()
	{
		this->context = cgCreateContext();
		vertex_profile = CG_PROFILE_VP40;
		fragment_profile = CG_PROFILE_FP40;
		// Need to have openGL loaded up for these functions to work, I think
		// vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
		// cgGLSetContextOptimalOptions(this->context, vertex_profile);
		// fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		// cgGLSetContextOptimalOptions(this->context, fragment_profile);
	}

	// Destructor
	cgGLShaderBackend::~cgGLShaderBackend()
	{
		cgDestroyContext(this->context);
	}

		const std::string& fragment_shader_path)
	{
		std::string vertex_src;
		this->readFile(vertex_shader_path, vertex_src);
		std::string fragment_src;
		this->readFile(fragment_shader_path, fragment_src);

		this->checkBackendForCgError("Initialising shader...");
		std::shared_ptr<ShaderObjects::cgGLShaderObject> shader_object = std::make_shared<ShaderObjects::cgGLShaderObject>(vertex_src, this->vertex_profile,
			fragment_src, this->fragment_profile,
			this->context);

		shader_object->load();
		this->checkBackendForCgError("Loading program...");

		return shader_object;
	}

	// createShaderProgramFromFiles
	std::shared_ptr<ShaderObjects::cgGLShaderObject> cgGLShaderBackend::createShaderProgramFromFiles(const std::string& vertex_shader_path,
		const std::string& fragment_shader_path)
	{
		std::string vertex_src;
		this->readFile(vertex_shader_path, vertex_src);
		std::string fragment_src;
		this->readFile(fragment_shader_path, fragment_src);

		return this->createShaderProgram(vertex_src, fragment_src);
	}


	// ReadFile
	const void cgGLShaderBackend::readFile(const std::string& filepath, std::string& out)
	{
		std::fstream filestream;
		try
		{
			filestream.open(filepath, std::ios::in);
			std::stringstream file_contents;
			file_contents << filestream.rdbuf();
			filestream.close();
			out = file_contents.str();
		}
		catch (const std::exception& e)
		{
			if (filestream.is_open())
				filestream.close();
			throw e;
		}
	}

	// checkForCgError
	const void cgGLShaderBackend::checkBackendForCgError(const std::string& message)
	{
		checkForCgError(this->context, message);
	}
}