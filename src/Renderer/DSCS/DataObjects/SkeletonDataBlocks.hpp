#pragma once

#include <array>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include "../../../Renderer/FileFormats/DSCS/SkelFile/SkelReadWrite.hpp"
#include "../../Utils/Hashing.hpp"
#include "../../Utils/Matrix.hpp"

namespace DataBlocks::Skeleton
{
	typedef uint16_t BONEID_t;
	typedef uint16_t SHADERID_t;
	typedef uint32_t OBJHASH_t;

	struct BoneDataBlock
	{
		std::array<float, 4> quat;
		std::array<float, 3> loc;
		std::array<float, 3> scale;
		std::array<float, 12> ibpm; // inverse_bind_pose_matrix
		BONEID_t parent;
		OBJHASH_t name_hash;
		std::string name;
		std::list<BONEID_t> children;
	};

	struct ShaderUniformChannelDataBlock
	{
		uint8_t flags;
		uint16_t channel_id;
		SHADERID_t shader_uniform_id;
		OBJHASH_t obj_hash;
	};

	class SkeletonDataBlock
	{
	public:
		std::vector<std::array<float, 4>> quat_buffer;
		std::vector<std::array<float, 3>> loc_buffer;
		std::vector<std::array<float, 3>> scale_buffer;
		std::vector<std::array<float, 12>> temp_transform_buffer;
		std::vector<std::array<float, 12>> transform_buffer;
		std::vector<BONEID_t> bone_parents;

		const auto& getBoneDataBlocks() const noexcept          { return this->bones; };
		const auto& getBoneIDMap() const noexcept               { return this->bone_hash_to_id; };
		const auto& getShaderChannelDataBlocks() const noexcept { return this->shader_uniform_channels; };
		const auto& getShaderChannelIDGroups() const noexcept   { return this->shader_channels_by_obj_hash; };

		BONEID_t addBoneDataBlock(std::string name, BONEID_t parent_id, const std::array<float, 12>& ibpm,
			                      const std::array<float, 4>& quat, const std::array<float, 3>& loc, const std::array<float, 3>& scale);
		void removeBoneDataBlock(BONEID_t bone_id);
		SHADERID_t addShaderChannelDataBlock(OBJHASH_t obj_hash, uint16_t channel_id, uint8_t flags);
		void removeShaderChannelDataBlock(SHADERID_t shader_id);

		void computeTransformBuffer();

	private:
		std::unordered_map<BONEID_t, BoneDataBlock> bones;
		std::unordered_map<SHADERID_t, ShaderUniformChannelDataBlock> shader_uniform_channels;
		std::unordered_map<OBJHASH_t, BONEID_t> bone_hash_to_id;
		std::unordered_map<OBJHASH_t, std::list<SHADERID_t>> shader_channels_by_obj_hash;

		std::queue<BONEID_t> spare_bone_ids;
		std::queue<SHADERID_t> spare_shader_channel_ids;

		static const BONEID_t root_id = -1;

	};
}