#pragma once

#include <array>
#include <cinttypes>
#include <string>

enum class RGBAChannel
{
	R,
	G,
	B,
	A
};

enum class MapType
{
	NONE,
	DIFFUSE,
	OLDIFFUSE,
	NORMAL,
	OLNORMAL,
	LIGHT,
	VERTEX
};

enum class Operation
{
	ADD,
	SUB,
	MUL,
	DIV
};


struct TexMap
{
	bool enabled = false;
	MapType type = MapType::NONE;
	RGBAChannel channel = RGBAChannel::A;
};

struct TexSlot
{
	bool enabled = false;
	uint8_t vec_size = 2;
	bool is_projection = false;
	bool scroll = false;
	bool offset = false;
	bool rotation = false;
	bool scale = false;
};

struct Sampler
{
	bool enabled = false;
	uint8_t uv_slot = 0;
	bool combined_channels = false;
};

struct TexLayer
{
	Sampler colorsampler;
	Sampler normalsampler;
};

struct FactorySettings
{
	// Vertex stuff
	bool use_normals = false;
	bool use_tangents = false;
	bool use_tangents_layer_2 = false;
	bool calculate_binormal = false;
	bool use_vertex_colors = false;
	bool use_weights = false;
	uint8_t num_vertex_weights = 0;
	bool is_billboard = false;
	bool align_with_y = false;
	bool use_skeleton = false;
	bool receive_shadows = false;
	bool apply_world_matrix = false;

	bool fat = false;
	bool zbias = false;

	// Fragment stuff
	bool view_matrix = false;
	bool inverse_view_matrix = false;
	bool requires_screenpos = false;
	bool glass_map = false;
	bool specular_input = false;
	bool use_specular = false;
	bool use_specular_alpha = false;
	bool use_reflection = false;
	bool use_velvet = false;
	bool use_fresnel = false;
	bool use_occlusion = false;
	bool use_clut = false;
	bool refl_sphere_map = false;
	bool use_obscure = false;
	bool use_ambient = false;
	bool use_ambient_gndsky = false;

	// Parallax
	bool parallax = false;
	MapType parallax_heightmap = MapType::DIFFUSE;
	RGBAChannel parallax_heightmap_channel = RGBAChannel::A;

	// Other stuff
	bool fog = false;
	bool fog_height = false;
	bool use_time = false;
	bool dir_light_1 = false;
	uint8_t num_lights = 0;
	uint8_t fragment_normal_vec_size = 3;

	std::array<TexSlot, 3> uv_slots;

	TexLayer texlayer_1;
	TexLayer texlayer_2;
	Sampler lightsampler;
	bool texlayer_1_distort = false;
	bool use_lightpower = false;

	// Transparency and vertex colours
	bool layer_1_vertex_rgb = false;
	bool layer_1_vertex_alpha = false;
	bool layer_2_vertex_alpha = false;
	MapType layer_1_transparency_map = MapType::NONE;
	RGBAChannel layer_1_transparency_map_channel = RGBAChannel::A;
	// TexMap transparency_map;
	TexMap diffuse_map;
	TexMap specular_map;
	TexMap layer_2_diffuse_map{ true, MapType::OLDIFFUSE, RGBAChannel::A };
	TexMap ambient_occlusion;
	Operation light_op = Operation::MUL;
};

bool uvSlotInUse(const Sampler& sampler, size_t idx);
void getUVSlot(FactorySettings& settings, size_t idx);
void createSettingInputs(FactorySettings& settings);
