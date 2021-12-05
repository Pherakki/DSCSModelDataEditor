#include "SkeletonDataBlocks.hpp"


namespace DataBlocks::Skeleton
{

	BONEID_t SkeletonDataBlock::addBoneDataBlock(std::string name, BONEID_t parent_id, const std::array<float, 12>& ibpm,
		const std::array<float, 4>& quat, const std::array<float, 3>& loc, const std::array<float, 3>& scale)
	{
		// Get a spare ID if one exists, else make a new one
		// Also destroy any unnecessary IDs encountered
		BONEID_t bone_id;
		bool is_new_bone = true;
		bone_id = this->bones.size();
		while (bone_id >= this->bones.size() && !this->spare_bone_ids.empty())
		{
			bone_id = this->spare_bone_ids.front();
			this->spare_bone_ids.pop();
			is_new_bone = false;
		}

		OBJHASH_t hash = dscsNameHash(name);
		this->bones.emplace(bone_id, BoneDataBlock{ quat, loc, scale, ibpm, parent_id, hash, name, {} });
		if (parent_id != (BONEID_t)-1)
			this->bones[parent_id].children.push_back(bone_id);
		this->bone_hash_to_id.emplace(hash, bone_id);
		this->bone_parents.insert(this->bone_parents.begin() + bone_id, parent_id);
		// If we've had to add a new slot...
		if (is_new_bone)
		{
			this->quat_buffer.push_back({ });
			this->loc_buffer.push_back({ });
			this->scale_buffer.push_back({ });
			this->temp_transform_buffer.push_back({ });
			this->transform_buffer.push_back({ });
		}

		return bone_id;
	}

	void SkeletonDataBlock::removeBoneDataBlock(BONEID_t bone_id)
	{
		auto& curr_bone = this->bones[bone_id];

		// Pass curr_bone's children to curr_bone's parent
		BONEID_t parent_id = curr_bone.parent;
		if (parent_id != root_id)
		{
			for (BONEID_t i : curr_bone.children)
			{
				this->bones[i].parent = parent_id;
				this->bones[parent_id].children.push_back(i);
			}
		}
		else
		{
			for (BONEID_t i : curr_bone.children)
				this->bones[i].parent = root_id;
		}

		// Now safe to remove the bone
		this->bone_hash_to_id.erase(curr_bone.name_hash);
		this->bones.erase(bone_id);

		// If it's the end of the list... pop off the end of the animation buffer since we don't need it any more
		// Else, keep the size of the buffer and keep track of the bone ID for later repurposing so we don't waste IDs
		if (bone_id == (this->bones.size() - 1))
		{
			this->quat_buffer.pop_back();
			this->loc_buffer.pop_back();
			this->scale_buffer.pop_back();
			this->temp_transform_buffer.pop_back();
			this->transform_buffer.pop_back();
		}
		else
			this->spare_bone_ids.push(bone_id);
	}

	SHADERID_t SkeletonDataBlock::addShaderChannelDataBlock(OBJHASH_t obj_hash, uint16_t channel_id, uint8_t flags)
	{
		// Get a spare ID if one exists, else get a new one
		SHADERID_t shader_id;
		if (this->spare_shader_channel_ids.empty())
			shader_id = this->shader_uniform_channels.size();
		else
		{
			shader_id = this->spare_shader_channel_ids.front();
			this->spare_shader_channel_ids.pop();
		}

		// Add the uniform channel
		this->shader_uniform_channels.emplace(shader_id, ShaderUniformChannelDataBlock{ flags, channel_id, shader_id, obj_hash });
		if (!this->shader_channels_by_obj_hash.contains(obj_hash))
			this->shader_channels_by_obj_hash.emplace(obj_hash, std::list<SHADERID_t>{});
		this->shader_channels_by_obj_hash[obj_hash].push_back(shader_id);

		return shader_id;
	}

	void SkeletonDataBlock::removeShaderChannelDataBlock(SHADERID_t shader_id)
	{
		OBJHASH_t hash = this->shader_uniform_channels[shader_id].obj_hash;

		this->shader_channels_by_obj_hash[hash].remove(shader_id);
		this->shader_uniform_channels.erase(shader_id);
	}

	void SkeletonDataBlock::computeTransformBuffer()
	{
		// Compute the individual bone matrices relative to their parents
		for (size_t i = 0; i < this->temp_transform_buffer.size(); ++i)
		{
			this->temp_transform_buffer[i] = transformsToMatrix(this->quat_buffer[i], this->loc_buffer[i], this->scale_buffer[i]);
		}

		// Compute bone matrices relative to model root
		for (size_t i = 0; i < this->temp_transform_buffer.size(); ++i)
		{
			this->transform_buffer[i] = makeTotalBoneMatrix(i, this->temp_transform_buffer, this->bone_parents);
		}

		// Compute total transforms for vertices by creating an operator that first undoes the bind pose, and then applies the new bone pose
		for (size_t i = 0; i < this->transform_buffer.size(); ++i)
		{
			this->transform_buffer[i] = multiplyTransformMatrices(this->transform_buffer[i], this->bones[i].ibpm);
		}
	}
}