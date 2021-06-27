#pragma once

#include <memory>
#include <vector>
#include <Rendering/DataObjects/OpenGLDSCSMesh.hpp>
#include <Rendering/DataObjects/OpenGLDSCSMaterial.hpp>

namespace Rendering::DataObjects
{
	class OpenGLDSCSModel
	{
	public:
		std::vector<std::unique_ptr<OpenGLDSCSMesh>> meshes;
		//std::vector<std::shared_ptr<OpenGLMaterial>> materials;
	};
}
