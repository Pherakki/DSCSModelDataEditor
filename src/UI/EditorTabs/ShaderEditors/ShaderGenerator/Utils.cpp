#include <iostream>

#include "Utils.hpp"


bool uvSlotInUse(const Sampler& sampler, size_t idx)
{
	return sampler.enabled && (sampler.uv_slot == idx);
}

void getUVSlot(FactorySettings& settings, size_t idx)
{
	settings.uv_slots[idx].enabled = uvSlotInUse(settings.texlayer_1.colorsampler, idx)
		|| uvSlotInUse(settings.texlayer_1.normalsampler, idx)
		|| uvSlotInUse(settings.texlayer_2.colorsampler, idx)
		|| uvSlotInUse(settings.texlayer_2.normalsampler, idx)
		|| uvSlotInUse(settings.lightsampler, idx);
}

void createSettingInputs(FactorySettings& settings)
{
	settings.use_vertex_colors = settings.layer_1_vertex_rgb || settings.layer_1_vertex_alpha || settings.layer_2_vertex_alpha;
	settings.use_tangents = settings.texlayer_1.normalsampler.enabled || settings.parallax;
	settings.use_tangents_layer_2 = settings.texlayer_2.normalsampler.enabled;
	settings.texlayer_1.normalsampler.enabled = settings.texlayer_1.normalsampler.enabled || settings.texlayer_1_distort || settings.transparency_map.type == MapType::NORMAL;
	settings.use_lightpower = settings.lightsampler.enabled;
	settings.lightsampler.enabled = settings.lightsampler.enabled || settings.transparency_map.type == MapType::LIGHT;

	getUVSlot(settings, 0);
	getUVSlot(settings, 1);
	getUVSlot(settings, 2);

	settings.use_ambient = settings.use_ambient || settings.use_ambient_gndsky;

	settings.use_normals = settings.use_reflection || settings.dir_light_1 || settings.use_tangents;

	settings.inverse_view_matrix = settings.is_billboard;
	settings.view_matrix = settings.specular_input || settings.use_reflection || settings.use_velvet || settings.parallax;
	settings.use_specular = settings.specular_input || settings.use_reflection;

	if (settings.use_normals && settings.fog)
		settings.fragment_normal_vec_size = 4;
	else if (settings.texlayer_1.colorsampler.enabled and settings.fog)
		settings.uv_slots[settings.texlayer_1.colorsampler.uv_slot].vec_size = 3;

	settings.requires_screenpos = settings.glass_map;

	settings.use_time = settings.uv_slots[0].scroll || settings.uv_slots[1].scroll || settings.uv_slots[2].scroll;

	settings.use_obscure = (settings.receive_shadows || settings.ambient_occlusion.enabled);
}
