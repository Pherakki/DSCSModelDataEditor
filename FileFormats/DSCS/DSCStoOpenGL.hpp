#pragma once

#include <FileFormats/DSCS/GeomFile/GeomReadWrite.hpp>
#include <FileFormats/DSCS/NameFile/NameReadWrite.hpp>
#include <FileFormats/DSCS/SkelFile/SkelReadWrite.hpp>

#include <Rendering/DataObjects/OpenGLDSCSModel.hpp>
#include <Rendering/DataObjects/OpenGLDSCSMaterial.hpp>

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <Rendering/ShaderBackends/cgGL/cgGLShaderBackend.hpp>


namespace FileFormats::DSCS
{
	Rendering::DataObjects::OpenGLDSCSModel DSCStoOpenGL(const std::filesystem::path& filepath, 
		                                                 std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend>& shader_backend,
		                                                 std::map<uint32_t, std::shared_ptr<Rendering::DataObjects::OpenGLDSCSMaterial>>& material_library,
		                                                 std::map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>>& shader_library)
	{
		NameFile::NameReadWrite name_file = NameFile::NameReadWrite();
		SkelFile::SkelReadWrite skel_file = SkelFile::SkelReadWrite();
		GeomFile::GeomReadWrite geom_file = GeomFile::GeomReadWrite();

		std::filesystem::path parent_directory = filepath.parent_path();

		
		std::filesystem::path geom_path = filepath;
		geom_path += ".geom";
		geom_file.read(geom_path.string());

		auto model = Rendering::DataObjects::OpenGLDSCSModel();

		// Materials
		auto model_materials = std::vector<std::shared_ptr<Rendering::DataObjects::OpenGLDSCSMaterial>>(geom_file.materials.size());
		for (int i = 0; i < geom_file.materials.size(); i++)
		{
			auto &geom_mat = geom_file.materials[i];
			auto material = std::make_shared<Rendering::DataObjects::OpenGLDSCSMaterial>();
			std::stringstream stream;
			stream << std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[0] << "_"
			       << std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[1] << "_"
				   << std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[2] << "_"
				   << std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[3];
			std::string shader_name(stream.str());
			
			std::filesystem::path shader_path = filepath;
			shader_path /= "shaders";
			shader_path /= shader_name;
			std::filesystem::path vp_path = shader_path;
			vp_path += "_vp.shad";
			std::filesystem::path fp_path = shader_path;
			fp_path += "_fp.shad";

			// shader_backend.createShaderProgram(vp_path.str(), fp_path.str());
			std::string shader_id = "common_shader";
			std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject> shader;
			if (shader_library.count(shader_id) == 0)
			{
				shader = shader_backend->createShaderProgram("shaders/red_vp.shad", "shaders/test_fp.shad");
				shader_library[shader_id] = shader;
			}
			else
				shader = shader_library[shader_id];
			material->shader = shader;

			//model_materials.push_back(material);
			material_library[geom_mat.name_hash] = material;
			model_materials[i] = material;

		}

		// Mesh
		for (int i = 0; i < geom_file.meshes.size(); i++)
		{
			auto &geom_mesh = geom_file.meshes[i];
			model.meshes.emplace_back(std::make_unique<Rendering::DataObjects::OpenGLDSCSMesh>(geom_mesh));
			auto& mesh = model.meshes[i];
			mesh->material = model_materials[geom_mesh.material_id];
		}

		return model;

	}
}