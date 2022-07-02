#pragma once


#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "../../../Cg/cg.h"
#include "../../../Cg/cgGL.h"

#include "cgGLShaderObject.hpp"
//#include "Rendering/DSCS/ShaderSystem/cgGL/Utils.hpp"

namespace Rendering::ShaderBackends
{
	// Should make this polymorphic eventually, but too much hassle for now
	class cgGLShaderBackend
	{
	private:
		CGcontext context;
		CGprofile vertex_profile;
		CGprofile fragment_profile;

		const void readFile(const std::string& filepath, std::string& out);
	public:
		cgGLShaderBackend();
		~cgGLShaderBackend();
		const void checkBackendForCgError(const std::string& message);
		std::shared_ptr<ShaderObjects::cgGLShaderObject> createShaderProgram(const std::string& vertex_shader_text, const std::string& fragment_shader_text);
		std::shared_ptr<ShaderObjects::cgGLShaderObject> createShaderProgramFromFiles(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
	};
}