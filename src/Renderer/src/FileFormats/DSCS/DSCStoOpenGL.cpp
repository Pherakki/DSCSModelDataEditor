#include "DSCStoOpenGL.hpp"
#include "../../Utils/Hashing.hpp"

// Init used textures
uint16_t initTexture(uint16_t uniform_id,const std::filesystem::path& img_path, const std::string& tex_name, TextureLibrary& texture_library)
{
	TextureType tex_type;
	TextureLibrary::TextureSubLibrary_t* sublib = nullptr;
	switch (uniform_id)
	{
	case 0x32:
	case 0x35:
	case 0x44:
	case 0x45:
	case 0x8E:
		tex_type = TextureType::Texture2D;
		sublib = &texture_library.textures_2d;
		break;
	case 0x48:
		tex_type = TextureType::TextureCLUT;
		sublib = &texture_library.textures_clut;
		break;
	case 0x3A:
		tex_type = TextureType::TextureCube;
		sublib = &texture_library.textures_cube;
		break;
	case 0x43:
		tex_type = TextureType::TextureLuminance;
		sublib = &texture_library.textures_luminance;
		break;
	default:
		std::string error_msg = "Unknown texture type ";
		error_msg += (char*)tex_type;
		error_msg += '.';
		throw std::runtime_error(error_msg.c_str());
		break;
	}

	if (sublib)
	{
		if (sublib->contains(tex_name))
		{
			return sublib->at(tex_name);
		}
		else
		{
			auto full_img_path = img_path;
			full_img_path /= (tex_name + ".img");
			GLuint tex_id = loadDDS(full_img_path.string().c_str(), tex_type);
			(*sublib)[tex_name] = tex_id;
			return tex_id;
		}
	}
	else
	{
		throw std::runtime_error("CRITICAL ERROR: Texture library was not set when loading DDS!");
	}
}



namespace FileFormats::DSCS
{

	// Should convert all data structures in here to use the "Data Blocks" prescription
	Rendering::DSCS::DataObjects::OpenGLDSCSModel DSCStoOpenGL(const std::filesystem::path& filepath,
		std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend>& shader_backend,
		const std::array<float*, 0xA0>& uniform_dispatch_buffer,
		TextureLibrary& texture_library,
		std::unordered_map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>>& shader_library)
	{
		NameFile::NameReadWrite name_file = NameFile::NameReadWrite();
		SkelFile::SkelReadWrite skel_file = SkelFile::SkelReadWrite();
		GeomFile::GeomReadWrite geom_file = GeomFile::GeomReadWrite();

		std::filesystem::path parent_directory = filepath.parent_path();
		std::filesystem::path img_path = parent_directory;
		img_path /= "images";

		// Read name
		std::filesystem::path name_path = filepath;
		name_path += ".name";
		// TODO: Add check if filepaths exist
		if (!std::filesystem::exists(name_path))
			throw std::runtime_error(("Path '" + name_path.string() + "' does not exist!").c_str());
		name_file.read(name_path.string());

		// Read skel
		std::filesystem::path skel_path = filepath;
		skel_path += ".skel";
		if (!std::filesystem::exists(skel_path))
			throw std::runtime_error(("Path '" + skel_path.string() + "' does not exist!").c_str());
		skel_file.read(skel_path.string());

		// Read geom
		std::filesystem::path geom_path = filepath;
		geom_path += ".geom";
		if (!std::filesystem::exists(geom_path))
			throw std::runtime_error(("Path '" + geom_path.string() + "' does not exist!").c_str());
		geom_file.read(geom_path.string());

		auto model = Rendering::DSCS::DataObjects::OpenGLDSCSModel();
		auto model_textures = std::vector<std::shared_ptr<Rendering::DataObjects::OpenGLDSCSTexture>>(geom_file.texture_names.size());
		// Textures
		// This isn't correct - needs to load uncompressed textures + cubemaps, and be told which is which by the DDS header - but this will do for now

		// Anim path

		std::filesystem::path anim_path = filepath;
		anim_path += ".anim";
		if (!std::filesystem::exists(anim_path))
			throw std::runtime_error(("Path '" + anim_path.string() + "' does not exist!").c_str());
		loadAnimation<true>(model, skel_file.getNumShaderChannels(), anim_path);

		// Materials
		model.materials = std::vector<std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>>(geom_file.materials.size());
		auto& material_names = name_file.getMaterialNames();
		std::unordered_map<uint32_t, std::string> material_name_lookup;
		for (auto& material_name : material_names)
		{
			material_name_lookup[dscsNameHash(material_name)] = material_name;
		}

		for (int i = 0; i < geom_file.materials.size(); ++i)
		{
			auto& geom_mat = geom_file.materials[i];

			// Get and Init Shaders
			std::stringstream stream;
			stream << std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[0] << "_"
				<< std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[1] << "_"
				<< std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[2] << "_"
				<< std::hex << std::setfill('0') << std::setw(8) << geom_mat.shader_hash[3];
			std::string shader_name(stream.str());

			std::filesystem::path shader_path = parent_directory;
			shader_path /= "shaders";
			shader_path /= shader_name;
			std::filesystem::path vp_path = shader_path;
			vp_path += "_vp.shad";
			std::filesystem::path fp_path = shader_path;
			fp_path += "_fp.shad";


			std::string shader_id = shader_name;
			std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject> shader;
			if (shader_library.contains(shader_id))
				shader = shader_library[shader_id];
			else
			{
				// TODO: Add check if filepaths exist
				shader = shader_backend->createShaderProgramFromFiles(vp_path.string(), fp_path.string());
				shader_library[shader_id] = shader;
			}

			// Init the material with the shader
			auto material = std::make_shared<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>(shader, uniform_dispatch_buffer);

			// Init Shader Uniforms handlers
			material->name_hash = geom_mat.name_hash;
			material->name = material_name_lookup[geom_mat.name_hash];
			// Copy shader uniform data into the material
			for (int j = 0; j < geom_mat.shader_uniforms.size(); ++j)
			{
				auto& geom_mat_uniform = geom_mat.shader_uniforms[j];
				if (geom_mat_uniform.num_floats_in_payload == 0)
				{
					std::array<uint16_t, 8> data;
					memcpy(&data, &geom_mat_uniform.payload, sizeof(geom_mat_uniform.payload));
					// TODO: Add check if filepaths exist
					material->setTextureBuffer(geom_mat_uniform.shader_uniform_type, initTexture(geom_mat_uniform.shader_uniform_type, img_path, geom_file.texture_names[data[0]].data(), texture_library));
					material->setTextureName(geom_mat_uniform.shader_uniform_type, &geom_file.texture_names[geom_mat_uniform.payload[0]][0]);
				}
				else
					material->setUniformValue(geom_mat_uniform.shader_uniform_type, geom_mat_uniform.payload);
				checkGLError();
			}

			// Init OpenGL settings
			for (int j = 0; j < geom_mat.opengl_settings.size(); ++j)
			{
				auto& geom_mat_ogl_setting = geom_mat.opengl_settings[j];
				material->addOpenGLSetting(geom_mat_ogl_setting.setting_type, geom_mat_ogl_setting.payload);
				checkGLError();
			}

			// Save the result
			model.materials[i] = material;
		}

		// Init skel data blocks
		auto& rest_pose = skel_file.getRestPoseData();
		auto& bone_parents = skel_file.getBoneParents();
		auto& bone_names = name_file.getBoneNames();
		auto& ibpms = geom_file.getIBPMs();
		for (uint16_t i = 0; i < skel_file.getNumBones(); ++i)
			model.skeleton.addBoneDataBlock(bone_names[i], bone_parents[i], ibpms[i],
				{ rest_pose[i][0], rest_pose[i][1], rest_pose[i][2], rest_pose[i][3] },
				{ rest_pose[i][4], rest_pose[i][5], rest_pose[i][6] },
				{ rest_pose[i][8], rest_pose[i][9], rest_pose[i][10] });

		auto& shader_channel_flags = skel_file.getShaderChannelFlags();
		auto& shader_channel_object_hashes = skel_file.getShaderChannelObjectHashes();
		auto& shader_channel_buffer_offsets = skel_file.getShaderChannelBufferOffsets();
		for (uint16_t i = 0; i < skel_file.getNumShaderChannels(); ++i)
			model.skeleton.addShaderChannelDataBlock(shader_channel_object_hashes[i], shader_channel_buffer_offsets[i], shader_channel_flags[i]);


		//for (int i = 0; i < rest_data.size(); ++i)
		//	model.skeleton[i] = multiplyTransformMatrices(makeTotalBoneMatrix(i, rest_data, bone_parents), geom_file.inverse_bind_pose_matrices[i]);

		// Mesh
		for (int i = 0; i < geom_file.meshes.size(); ++i)
		{
			auto& geom_mesh = geom_file.meshes[i];
			model.meshes.emplace_back(std::make_shared<Rendering::DSCS::DataObjects::OpenGLDSCSMesh>(geom_mesh));
			auto& mesh = model.meshes[i];
			mesh->material = model.materials[geom_mesh.material_id];
			mesh->used_bones = geom_mesh.skinning_indices;
		}

		return model;

	}

}