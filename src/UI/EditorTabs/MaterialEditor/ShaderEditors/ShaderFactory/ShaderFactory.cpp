#include "ShaderFactory.hpp"

// ################# //
// Utility functions //
// ################# //

float ShaderFactory::safeStof(const QString& value) noexcept
{
	float f_value;
	try
	{
		f_value = std::stof(value.toStdString());
	}
	catch (const std::exception& ex)
	{
		f_value = 0;
	}
	return f_value;
}

std::array<float, 4> ShaderFactory::sanitiseTextChanged(const std::vector<EditorTextbox*>& textboxes)
{
	std::array<float, 4> out{};
	for (size_t i = 0; i < textboxes.size(); ++i)
		out[i] = ShaderFactory::safeStof(textboxes[i]->text());
	return out;
}

float ShaderFactory::sanitiseTextChanged(EditorTextbox* textbox)
{
	return ShaderFactory::safeStof(textbox->text());
}

std::string ShaderFactory::hashShader()
{
	auto& mat = this->selected_objects.getEditableSelectedMaterialResource().getFactoryMaterial();
	auto md5 = QCryptographicHash(QCryptographicHash::Algorithm::Md5);
	auto vp_hash = md5.hash(mat->shader->vertex_source.c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();
	auto fp_hash = md5.hash(mat->shader->fragment_source.c_str(), QCryptographicHash::Algorithm::Md5).toHex().toStdString();
	return vp_hash + fp_hash;
}

// #################################### //
// Functions used with applyOnTextboxes //
// #################################### //

void ShaderFactory::assignUniformValues(MaterialPtr& material)
{
	struct Functor
	{
		ShaderFactory& obj;
		MaterialPtr& material;
		Functor(ShaderFactory& obj, MaterialPtr& material)
			: obj{ obj }
			, material{ material }
		{};

		void operator()(size_t idx, EditorTextbox* textbox) const
		{
			material->setUniformValue(idx, { obj.sanitiseTextChanged(textbox) });
		}

		void operator()(size_t idx, const std::vector<EditorTextbox*>& textboxes) const
		{
			material->setUniformValue(idx, obj.sanitiseTextChanged(textboxes));
		}
	};

	this->applyOnTextboxes(Functor(*this, material));
}

void ShaderFactory::readbackUniformValues(const MaterialPtr& material)
{
	struct Functor
	{
		ShaderFactory& obj;
		const MaterialPtr& material;
		Functor(ShaderFactory& obj, const MaterialPtr& material)
			: obj{ obj }
			, material{ material }
		{};

		void readbackUniformToUI(const MaterialPtr& material, int textbox_idx, size_t idx, EditorTextbox* textbox) const
		{
			auto& buf = material->local_uniform_buffer;
			std::stringstream stream;
			stream << std::fixed << std::setprecision(textbox->getNumDigits()) << buf[idx][textbox_idx];
			textbox->clear();
			textbox->insert(QString::fromStdString(stream.str()));
			textbox->setCursorPosition(0);
		}

		void operator()(size_t idx, EditorTextbox* textbox) const
		{
			this->readbackUniformToUI(this->material, 0, idx, textbox);
		}

		void operator()(size_t idx, const std::vector<EditorTextbox*>& textboxes) const
		{
			for (int i = 0; i < textboxes.size(); ++i)
				this->readbackUniformToUI(this->material, i, idx, textboxes[i]);
		}
	};

	this->applyOnTextboxes(Functor(*this, material));
}

void ShaderFactory::hookUniformValueUpdate(size_t idx, EditorTextbox* textbox)
{
	connect(textbox, &EditorTextbox::textChanged, this, [this, idx, textbox]()
		{
			auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
			auto& material = material_resource.getFactoryMaterial();
			material->setUniformValue(idx, { this->sanitiseTextChanged(textbox) });
			if (material_resource.isFactoryActive())
				material_resource.updateMainMaterialBuffer(idx, { this->sanitiseTextChanged(textbox) });
		}
	);
}

void ShaderFactory::hookUniformValueUpdate(size_t idx, const std::vector<EditorTextbox*>& textboxes)
{
	for (auto& textbox : textboxes)
		connect(textbox, &EditorTextbox::textChanged, this, [this, idx, textboxes]()
			{
				auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
				auto& material = material_resource.getFactoryMaterial();
				material->setUniformValue(idx, this->sanitiseTextChanged(textboxes));
				if (material_resource.isFactoryActive())
					material_resource.updateMainMaterialBuffer(idx, this->sanitiseTextChanged(textboxes));
			}
	);
}

void ShaderFactory::hookUniformUpdates()
{
	struct Functor
	{
		ShaderFactory& obj;
		Functor(ShaderFactory& obj) : obj{ obj } {};

		void operator()(size_t idx, EditorTextbox* textbox) const
		{
			this->obj.hookUniformValueUpdate(idx, textbox);
		}

		void operator()(size_t idx, const std::vector<EditorTextbox*>& textboxes) const
		{
			this->obj.hookUniformValueUpdate(idx, textboxes);
		}
	};

	this->applyOnTextboxes(Functor(*this));
}


// ################### //
// Shader Update Chain //
// ################### //


void ShaderFactory::blockUISignals(bool active)
{
	this->uv_settings_1->blockSignals(active);
	this->uv_settings_2->blockSignals(active);
	this->uv_settings_3->blockSignals(active);
	this->texture_layer_1->blockSignals(active);
	this->texture_layer_2->blockSignals(active);
	this->diffuse_color_settings->blockSignals(active);
	this->specular_color_settings->blockSignals(active);
	this->illumination_settings->blockSignals(active);
	this->reflection_settings->blockSignals(active);
	this->glassmap_settings->blockSignals(active);
	this->position_settings->blockSignals(active);

	this->uv_settings_1->widget_scrollspeed->textboxes[0]->blockSignals(active);
	this->uv_settings_1->widget_scrollspeed->textboxes[1]->blockSignals(active);
	this->uv_settings_1->widget_offset->textboxes[0]->blockSignals(active);
	this->uv_settings_1->widget_offset->textboxes[1]->blockSignals(active);
	this->uv_settings_1->widget_rotation->textboxes[0]->blockSignals(active);
	this->uv_settings_1->widget_scale->textboxes[0]->blockSignals(active);
	this->uv_settings_1->widget_scale->textboxes[1]->blockSignals(active);
	this->uv_settings_2->widget_scrollspeed->textboxes[0]->blockSignals(active);
	this->uv_settings_2->widget_scrollspeed->textboxes[1]->blockSignals(active);
	this->uv_settings_2->widget_offset->textboxes[0]->blockSignals(active);
	this->uv_settings_2->widget_offset->textboxes[1]->blockSignals(active);
	this->uv_settings_2->widget_rotation->textboxes[0]->blockSignals(active);
	this->uv_settings_2->widget_scale->textboxes[0]->blockSignals(active);
	this->uv_settings_2->widget_scale->textboxes[1]->blockSignals(active);
	this->uv_settings_3->widget_scrollspeed->textboxes[0]->blockSignals(active);
	this->uv_settings_3->widget_scrollspeed->textboxes[1]->blockSignals(active);
	this->uv_settings_3->widget_offset->textboxes[0]->blockSignals(active);
	this->uv_settings_3->widget_offset->textboxes[1]->blockSignals(active);
	this->uv_settings_3->widget_rotation->textboxes[0]->blockSignals(active);
	this->uv_settings_3->widget_scale->textboxes[0]->blockSignals(active);
	this->uv_settings_3->widget_scale->textboxes[1]->blockSignals(active);

	this->texture_layer_1->parallax_settings->bias_xy->textboxes[0]->blockSignals(active);
	this->texture_layer_1->parallax_settings->bias_xy->textboxes[1]->blockSignals(active);
	this->texture_layer_1->distortion->textboxes[0]->blockSignals(active);
	this->texture_layer_1->bumpmap_settings->bump_strength->textboxes[0]->blockSignals(active);
	this->texture_layer_2->bumpmap_settings->bump_strength->textboxes[0]->blockSignals(active);
	this->texture_layer_2->overlay_strength->textboxes[0]->blockSignals(active);
	this->diffuse_color_settings->diffuse_color_widget->textboxes[0]->blockSignals(active);
	this->diffuse_color_settings->diffuse_color_widget->textboxes[1]->blockSignals(active);
	this->diffuse_color_settings->diffuse_color_widget->textboxes[2]->blockSignals(active);
	this->diffuse_color_settings->diffuse_color_widget->textboxes[3]->blockSignals(active);
	this->diffuse_color_settings->light_power_textbox->textboxes[0]->blockSignals(active);
	this->diffuse_color_settings->light_strength_textbox->textboxes[0]->blockSignals(active);
	this->specular_color_settings->specular_power->textboxes[0]->blockSignals(active);
	this->specular_color_settings->specular_strength->textboxes[0]->blockSignals(active);
	this->reflection_settings->reflection_strength->textboxes[0]->blockSignals(active);
	this->reflection_settings->fresnel_exp->textboxes[0]->blockSignals(active);
	this->reflection_settings->fresnel_min->textboxes[0]->blockSignals(active);
	this->illumination_settings->surface_color->textboxes[0]->blockSignals(active);
	this->illumination_settings->surface_color->textboxes[1]->blockSignals(active);
	this->illumination_settings->surface_color->textboxes[2]->blockSignals(active);
	this->illumination_settings->fuzzy_spec_color->textboxes[0]->blockSignals(active);
	this->illumination_settings->fuzzy_spec_color->textboxes[1]->blockSignals(active);
	this->illumination_settings->fuzzy_spec_color->textboxes[2]->blockSignals(active);
	this->illumination_settings->sub_color->textboxes[0]->blockSignals(active);
	this->illumination_settings->sub_color->textboxes[1]->blockSignals(active);
	this->illumination_settings->sub_color->textboxes[2]->blockSignals(active);
	this->illumination_settings->rolloff->textboxes[0]->blockSignals(active);
	this->illumination_settings->velvet_strength->textboxes[0]->blockSignals(active);
	this->position_settings->zbias->textboxes[0]->blockSignals(active);
	this->position_settings->fat->textboxes[0]->blockSignals(active);
}

void ShaderFactory::updateUI()
{
	this->blockUISignals(true);
	std::cout << "UPDATE UI" << std::endl;
	if (this->setting_update_in_progress)
		return;
	this->setting_update_in_progress = true;


	// Get the UV slots in use
	this->uv_settings_1->toggle(getUVSlot(0));
	this->uv_settings_2->toggle(getUVSlot(1));
	this->uv_settings_3->toggle(getUVSlot(2));

	// Diffuse 1
	if (this->texture_layer_1->diffuse_texture_settings->isActive())
	{
		this->diffuse_color_settings->transparency_map_widget->addDiffuseMap();
		this->diffuse_color_settings->diffuse_map_widget->addDiffuseMap();
		this->specular_color_settings->specular_map->addDiffuseMap();
		this->illumination_settings->ambient_occlusion_map->addDiffuseMap();
	}
	else
	{
		this->diffuse_color_settings->transparency_map_widget->removeDiffuseMap();
		this->diffuse_color_settings->diffuse_map_widget->removeDiffuseMap();
		this->specular_color_settings->specular_map->removeDiffuseMap();
		this->illumination_settings->ambient_occlusion_map->removeDiffuseMap();
	}

	// Normal 1
	if (this->texture_layer_1->normal_texture_settings->isActive())
	{
		this->diffuse_color_settings->transparency_map_widget->addNormalMap();
		this->diffuse_color_settings->diffuse_map_widget->addNormalMap();
		this->specular_color_settings->specular_map->addNormalMap();
		this->illumination_settings->ambient_occlusion_map->addNormalMap();
	}
	else
	{
		this->diffuse_color_settings->transparency_map_widget->removeNormalMap();
		this->diffuse_color_settings->diffuse_map_widget->removeNormalMap();
		this->specular_color_settings->specular_map->removeNormalMap();
		this->illumination_settings->ambient_occlusion_map->removeNormalMap();
	}

	// Light
	if (this->diffuse_color_settings->light_sampler->isActive())
	{
		this->specular_color_settings->specular_map->addLightMap();
		this->illumination_settings->ambient_occlusion_map->addLightMap();
	}
	else
	{
		this->specular_color_settings->specular_map->removeLightMap();
		this->illumination_settings->ambient_occlusion_map->removeLightMap();
	}

	this->texture_layer_2->diffuse_texture_settings->setEnabled(this->texture_layer_1->diffuse_texture_settings->isActive());

	// Diffuse 2
	if (this->texture_layer_2->diffuse_texture_settings->isActive())
	{
		this->diffuse_color_settings->diffuse_map_widget_layer_2->addDiffuseMap<true>();
	}
	else
	{
		this->diffuse_color_settings->diffuse_map_widget_layer_2->removeDiffuseMap();
	}

	// Normal 2
	if (this->texture_layer_2->normal_texture_settings->isActive())
	{
		this->diffuse_color_settings->diffuse_map_widget_layer_2->addNormalMap<true>();
	}
	else
	{
		this->diffuse_color_settings->diffuse_map_widget_layer_2->removeNormalMap();
	}

	this->diffuse_color_settings->transparency_map_widget->setActive();
	this->diffuse_color_settings->diffuse_map_widget->setActive();
	this->diffuse_color_settings->diffuse_map_widget_layer_2->setActive();

	this->specular_color_settings->specular_power->setEnabled(!this->illumination_settings->clut->isActive());

	this->regenerateMaterial();
	this->blockUISignals(false);
}

bool ShaderFactory::uvSlotInUse(ShaderFactoryTextureSlot*& slot, size_t idx)
{
	return slot->isActive() && (slot->uv_slot_combobox->currentIndex() == idx);
}

bool ShaderFactory::getUVSlot(size_t idx)
{
	return this->uvSlotInUse(this->texture_layer_1->diffuse_texture_settings, idx)
		|| this->uvSlotInUse(this->texture_layer_1->normal_texture_settings, idx)
		|| this->uvSlotInUse(this->texture_layer_2->diffuse_texture_settings, idx)
		|| this->uvSlotInUse(this->texture_layer_2->normal_texture_settings, idx)
		|| this->uvSlotInUse(this->diffuse_color_settings->light_sampler, idx);
}

void ShaderFactory::regenerateMaterial()
{
	auto& settings = this->selected_objects.getEditableSelectedMaterialResource().getEditableFactorySettings();
	TextureRefs textures;
	this->createSettingsFromUI(settings, textures);

	// Need to figure out a way to do this with the new resource system...
	// Probably need to store a list of Mesh references on the MaterialResource..?
	auto& selected_mesh = this->selected_objects.getSelectedMesh();
	// Sort out remaining inputs to the factory
	bool found_weight = false;
	for (const auto& attr : selected_mesh->mesh.vertex_attributes)
	{
		if (attr.attribute_type == FileFormats::DSCS::GeomFile::VertexAttributeType::WeightedBoneID)
		{
			const auto& num_idxs = attr.num_elements;
			settings.num_vertex_weights = num_idxs;
			settings.use_skeleton = true;
			settings.use_weights = true;
			found_weight = true;
			break;

		}
	}
	if (!found_weight)
	{
		for (const auto& attr : selected_mesh->mesh.vertex_attributes)
		{
			if (attr.attribute_type == FileFormats::DSCS::GeomFile::VertexAttributeType::Position)
			{
				const auto& num_idxs = attr.num_elements;
				if (num_idxs == 4)
					settings.num_vertex_weights = 1;
				else
					settings.num_vertex_weights = 0;
				settings.use_skeleton = true;
				settings.use_weights = false;
				break;
			}
		}
	}

	createSettingInputs(settings);
	auto vshader_text = generateVertexShader(settings);
	auto fshader_text = generateFragmentShader(settings);
	std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject> shader;
	try
	{
		shader = this->shader_backend->createShaderProgram(vshader_text, fshader_text);
		this->shader_backend->checkBackendForCgError("Compiling shaders...");
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		std::cout << vshader_text << std::endl;
		std::cout << fshader_text << std::endl;
		return;
	}

	auto& material_resource = this->selected_objects.getEditableSelectedMaterialResource();
	auto& curr_material = material_resource.getEditableFactoryMaterial();
	curr_material->replaceShader(shader, this->animation_buffer.uniform_dispatch_buffer);
	this->assignTextureReferences(curr_material, textures);
	this->assignUniformValues(curr_material);

	if (material_resource.isFactoryActive())
		material_resource.updateMainMaterial(this->animation_buffer);

	this->setting_update_in_progress = false;

	auto& shader_hashes = this->selected_objects.getShaderHashes();
	auto factory_hash = this->hashShader();
	if (shader_hashes.contains(factory_hash))
		this->matching_shader->setText(QString::fromStdString("Matches: " + shader_hashes.at(factory_hash)[0]));
	else
		this->matching_shader->setText("No matching shader.");
}


void ShaderFactory::assignTextureReferences(MaterialPtr& material, TextureRefs& textures)
{
	if (textures.c1_texture)
	{
		material->setTextureBuffer(0x32, textures.c1_texture);
		material->setTextureName(0x32, textures.c1_texture_name);
	}
	if (textures.n1_texture)
	{
		material->setTextureBuffer(0x35, textures.n1_texture);
		material->setTextureName(0x35, textures.n1_texture_name);
	}
	if (textures.light_texture)
	{
		material->setTextureBuffer(0x43, textures.light_texture);
		material->setTextureName(0x43, textures.light_texture_name);
	}
	if (textures.c2_texture)
	{
		material->setTextureBuffer(0x44, textures.c2_texture);
		material->setTextureName(0x44, textures.c2_texture_name);
	}
	if (textures.n2_texture)
	{
		material->setTextureBuffer(0x45, textures.n2_texture);
		material->setTextureName(0x45, textures.n2_texture_name);
	}
	if (textures.clut_texture)
	{
		material->setTextureBuffer(0x48, textures.clut_texture);
		material->setTextureName(0x48, textures.clut_texture_name);
	}
	if (textures.env_texture)
	{
		if (textures.isSphere)
		{
			material->setTextureBuffer(0x8E, textures.env_texture);
			material->setTextureName(0x8E, textures.env_texture_name);
		}
		else
		{
			material->setTextureBuffer(0x3A, textures.env_texture);
			material->setTextureName(0x3A, textures.env_texture_name);
		}
	}
}

void ShaderFactory::createSettingsFromUI(FactorySettings& settings, TextureRefs& textures)
{
	// Load textures
	if (const auto& tex_ui = this->texture_layer_1->diffuse_texture_settings; tex_ui->isActive())
		this->createTexSettings2D(settings, settings.texlayer_1.colorsampler, textures.c1_texture_name, textures.c1_texture, *tex_ui);
	if (const auto& tex_ui = this->texture_layer_1->normal_texture_settings; tex_ui->isActive())
		this->createTexSettings2D(settings, settings.texlayer_1.normalsampler, textures.n1_texture_name, textures.n1_texture, *tex_ui);
	if (const auto& tex_ui = this->texture_layer_2->diffuse_texture_settings; tex_ui->isActive())
		this->createTexSettings2D(settings, settings.texlayer_2.colorsampler, textures.c2_texture_name, textures.c2_texture, *tex_ui);
	if (const auto& tex_ui = this->texture_layer_2->normal_texture_settings; tex_ui->isActive())
		this->createTexSettings2D(settings, settings.texlayer_2.normalsampler, textures.n2_texture_name, textures.n2_texture, *tex_ui);
	if (const auto& tex_ui = this->diffuse_color_settings->light_sampler; tex_ui->isActive())
		this->createTexSettingsLum(settings, settings.lightsampler, textures.light_texture_name, textures.light_texture, *tex_ui);
	if (const auto& tex_ui = this->illumination_settings->clut; tex_ui->isActive())
	{
		auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
		textures.clut_texture_name = texname;
		textures.clut_texture = this->selected_objects.texture_library.textures_clut.at(texname);
	}
	if (const auto& tex_ui = this->reflection_settings->env_texture; tex_ui->isActive())
	{
		auto texname = tex_ui->file_combo_box->combobox->currentText().toStdString();
		textures.env_texture_name = texname;
		if (this->reflection_settings->isSphereMap())
			textures.env_texture = this->selected_objects.texture_library.textures_2d.at(texname);
		else
			textures.env_texture = this->selected_objects.texture_library.textures_cube.at(texname);
		textures.isSphere = this->reflection_settings->isSphereMap();
	}

	// Handle texure later specific things
	settings.texlayer_1_distort = this->texture_layer_1->distortion->isActive();
	settings.texlayer_2.colorsampler.enabled = this->texture_layer_2->overlay_strength->isActive();

	// Handle UV adjustments
	this->createUVSettings(settings.uv_slots[0], *this->uv_settings_1);
	this->createUVSettings(settings.uv_slots[1], *this->uv_settings_2);
	this->createUVSettings(settings.uv_slots[2], *this->uv_settings_3);

	// Parallax
	this->createParallaxSettings(settings);
	this->createBumpSettings(settings);

	// Various Color Contributions
	this->createDiffuseColorSettings(settings);
	this->createSpecularColorSettings(settings);
	this->createIlluminationSettings(settings);
	this->createReflectionSettings(settings);

	// Vertex
	this->createVertexSettings(settings);

}

void ShaderFactory::createTexSettings(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui, TextureLibrary::TextureSubLibrary_t& sublib)
{
	sampler.enabled = true;
	sampler.uv_slot = tex_ui.uv_slot_combobox->currentIndex();
	settings.uv_slots[sampler.uv_slot].enabled = true;
	sampler.combined_channels = true; // Change later
	texname = tex_ui.file_combo_box->combobox->currentText().toStdString();
	texture = sublib.at(texname);
}

void ShaderFactory::createTexSettings2D(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui)
{
	this->createTexSettings(settings, sampler, texname, texture, tex_ui, this->selected_objects.texture_library.textures_2d);
}

void ShaderFactory::createTexSettingsLum(FactorySettings& settings, Sampler& sampler, std::string& texname, uint32_t& texture, const ShaderFactoryTextureSlot& tex_ui)
{
	this->createTexSettings(settings, sampler, texname, texture, tex_ui, this->selected_objects.texture_library.textures_luminance);
}

void ShaderFactory::createUVSettings(TexSlot& uv_slot, const ShaderFactoryUVSettingsWidget& uv_ui)
{
	uv_slot.is_projection = uv_ui.widget_projection->isActive();
	uv_slot.scroll = uv_ui.widget_scrollspeed->isActive();
	uv_slot.offset = uv_ui.widget_offset->isActive();
	uv_slot.rotation = uv_ui.widget_rotation->isActive();
	uv_slot.scale = uv_ui.widget_scale->isActive();
}

void ShaderFactory::createParallaxSettings(FactorySettings& settings)
{
	if (this->texture_layer_1->parallax_settings->isActive())
	{
		settings.parallax = true;
		settings.view_matrix = true;
		settings.use_tangents = true;
		auto heightmap = this->texture_layer_1->parallax_settings->heightmap_combobox->currentText();
		if (heightmap == "Diffuse Texture")
			settings.parallax_heightmap = MapType::DIFFUSE;
		else if (heightmap == "Normal Texture")
			settings.parallax_heightmap = MapType::NORMAL;
		else
			throw std::runtime_error("Error: parallax maptype not diffuse or normal.");
		settings.parallax_heightmap_channel = RGBAChannel::A; // Make this configurable
	}
}

void ShaderFactory::createDiffuseColorSettings(FactorySettings& settings)
{
	if (this->diffuse_color_settings->vertex_colors_widget->isActive())
	{
		settings.use_vertex_colors = true;
		auto vertex_color_setting = this->diffuse_color_settings->vertex_colors_widget->combobox->currentText();
		if (vertex_color_setting == "RGBA")
		{
			settings.layer_1_vertex_rgb = true;
			settings.layer_1_vertex_alpha = true;
		}
		else if (vertex_color_setting == "RGB")
		{
			settings.layer_1_vertex_rgb = true;
		}
		else
		{
			throw std::runtime_error("Unknown vertex color setting");
		}
	}

	if (this->diffuse_color_settings->transparency_map_widget->isActive())
	{
		settings.transparency_map.enabled = true;
		settings.transparency_map.type = this->diffuse_color_settings->transparency_map_widget->getMapType();
		settings.transparency_map.channel = this->diffuse_color_settings->transparency_map_widget->getChannel();
	}
	if (this->diffuse_color_settings->diffuse_map_widget->isActive())
	{
		settings.diffuse_map.enabled = true;
		settings.diffuse_map.type = this->diffuse_color_settings->diffuse_map_widget->getMapType();
		settings.diffuse_map.channel = this->diffuse_color_settings->diffuse_map_widget->getChannel();
	}
	if (this->diffuse_color_settings->diffuse_map_widget_layer_2->isActive())
	{
		settings.layer_2_diffuse_map.enabled = true;
		settings.layer_2_diffuse_map.type = this->diffuse_color_settings->diffuse_map_widget_layer_2->getMapType();
		settings.layer_2_diffuse_map.channel = this->diffuse_color_settings->diffuse_map_widget_layer_2->getChannel();
	}
}

void ShaderFactory::createSpecularColorSettings(FactorySettings& settings)
{
	settings.specular_input = this->specular_color_settings->checkbox->isChecked();
	if (this->specular_color_settings->specular_map->isActive())
	{
		settings.specular_map.enabled = true;
		settings.specular_map.type = this->specular_color_settings->specular_map->getMapType();
		settings.specular_map.channel = this->specular_color_settings->specular_map->getChannel();
	}
}

void ShaderFactory::createIlluminationSettings(FactorySettings& settings)
{
	settings.dir_light_1 = this->illumination_settings->receive_lamp->isActive();
	if (settings.dir_light_1)
	{
		settings.use_velvet = this->illumination_settings->velvet->isActive();
		settings.use_clut = this->illumination_settings->clut->isActive();
	}
	settings.use_ambient = this->illumination_settings->receive_ambient->isActive();
	settings.use_ambient_gndsky = this->illumination_settings->receive_sky->isActive();

	settings.fog = this->illumination_settings->receive_fog->isActive();
	settings.fog_height = this->illumination_settings->receive_fog->isActive();

	settings.use_obscure = this->illumination_settings->obscure->isActive();
	settings.ambient_occlusion.enabled = this->illumination_settings->ambient_occlusion_map->isActive();
	settings.ambient_occlusion.type = this->illumination_settings->ambient_occlusion_map->getMapType();
	settings.ambient_occlusion.channel = this->illumination_settings->ambient_occlusion_map->getChannel();
	settings.receive_shadows = this->illumination_settings->receive_shadow_map->isActive();
}

void ShaderFactory::createReflectionSettings(FactorySettings& settings)
{
	settings.use_reflection = this->reflection_settings->isActive();
	settings.refl_sphere_map = this->reflection_settings->isSphereMap();
	settings.use_fresnel = this->reflection_settings->fresnel->isActive();
}

void ShaderFactory::createBumpSettings(FactorySettings& settings)
{
	if (this->texture_layer_1->normal_texture_settings->isActive())
	{
		settings.use_tangents = true;
	}
}


void ShaderFactory::createVertexSettings(FactorySettings& settings)
{
	settings.calculate_binormal = this->position_settings->calculate_binormal;
	settings.fat = this->position_settings->fat->isActive();
	settings.zbias = this->position_settings->zbias->isActive();
	settings.is_billboard = this->position_settings->billboard_checkbox->isActive();
	settings.align_with_y = this->position_settings->billboard_align_y_checkbox->isActive();
}

// ##################### //
// UI Readback Functions //
// ##################### //

void ShaderFactory::updateReadbackSettings()
{
	this->blockUISignals(true);
	this->updateAvailableTextures(); // This should update in other places
	this->readbackUISettings();

	this->blockUISignals(false);
}

void ShaderFactory::readbackUISettings()
{
	auto& selected_material = this->selected_objects.getEditableSelectedMaterialResource().getFactoryMaterial();
	for (const auto& [id, texture] : selected_material->texture_refs)
	{
		if (texture->texture_type == CG_SAMPLER2D)
		{
			QComboBox* cbox = nullptr;
			switch (id)
			{
			case 0x32:
				cbox = this->texture_layer_1->diffuse_texture_settings->file_combo_box->combobox;
				break;
			case 0x35:
				cbox = this->texture_layer_1->normal_texture_settings->file_combo_box->combobox;
				break;
			case 0x43:
				cbox = this->diffuse_color_settings->light_sampler->file_combo_box->combobox;
				break;
			case 0x44:
				cbox = this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox;
				break;
			case 0x45:
				cbox = this->texture_layer_2->normal_texture_settings->file_combo_box->combobox;
				break;
			case 0x48:
				cbox = this->illumination_settings->clut->file_combo_box->combobox;
				break;
			default:
				return;
			}
			cbox->setCurrentText(QString::fromStdString(texture->img_name));
		}
	}
	this->readbackUniformValues(selected_material);
}


// ######################## //
// Texture Update Functions //
// ######################## //

void ShaderFactory::updateTexturesOn(QComboBox* combobox, TextureLibrary::TextureSubLibrary_t& tex_sublib)
{
	combobox->clear();
	for (const auto& [texture_name, texture] : tex_sublib)
	{
		combobox->addItem(QString::fromStdString(texture_name), QString::fromStdString(texture_name));
	}
}

void ShaderFactory::update2DTexturesOn(QComboBox* combobox)
{
	this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_2d);
}
void ShaderFactory::updateCLUTTexturesOn(QComboBox* combobox)
{
	this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_clut);
}
void ShaderFactory::updateLumTexturesOn(QComboBox* combobox)
{
	this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_luminance);
}
void ShaderFactory::updateCubeTexturesOn(QComboBox* combobox)
{
	this->updateTexturesOn(combobox, this->selected_objects.texture_library.textures_cube);
}

void ShaderFactory::updateAvailableTextures()
{
	this->update2DTexturesOn(this->texture_layer_1->diffuse_texture_settings->file_combo_box->combobox);
	this->update2DTexturesOn(this->texture_layer_1->normal_texture_settings->file_combo_box->combobox);
	this->update2DTexturesOn(this->texture_layer_2->diffuse_texture_settings->file_combo_box->combobox);
	this->update2DTexturesOn(this->texture_layer_2->normal_texture_settings->file_combo_box->combobox);
	this->updateLumTexturesOn(this->diffuse_color_settings->light_sampler->file_combo_box->combobox);
	this->updateCLUTTexturesOn(this->illumination_settings->clut->file_combo_box->combobox);
	if (this->reflection_settings->isSphereMap())
		this->update2DTexturesOn(this->reflection_settings->env_texture->file_combo_box->combobox);
	else
		this->updateCubeTexturesOn(this->reflection_settings->env_texture->file_combo_box->combobox);
}

// ############ //
// UI Functions //
// ############ //

void ShaderFactory::activateMaterial()
{
	this->selected_objects.getEditableSelectedMaterialResource().activateFactory();
	this->regenerateMaterial();
}

void ShaderFactory::placeInSpoiler(const QString& title, QWidget* widget, QLayout* layout)
{
	auto* spoiler = new Spoiler(title, 300, this);
	spoiler->setContentLayout(*widget->layout());
	layout->addWidget(spoiler);
}

void ShaderFactory::connectUI()
{
	connectUIUpdate(this->texture_layer_1);
	connectUIUpdate(this->texture_layer_2);
	connectUIUpdate(this->uv_settings_1);
	connectUIUpdate(this->uv_settings_2);
	connectUIUpdate(this->uv_settings_3);
	connectUIUpdate(this->diffuse_color_settings);
	connectUIUpdate(this->specular_color_settings);
	connectUIUpdate(this->reflection_settings);
	connectUIUpdate(this->illumination_settings);
	connectUIUpdate(this->glassmap_settings);
	connectUIUpdate(this->position_settings);
}

#include "moc_ShaderFactory.cpp"
