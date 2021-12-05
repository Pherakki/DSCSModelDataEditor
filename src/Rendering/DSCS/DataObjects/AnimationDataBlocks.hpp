#pragma once


#include <array>
#include <unordered_map>
#include <vector>

namespace DataBlocks::Animation
{
	template<typename T>
	class AnimationCurveDataBlock
	{
	public:
		AnimationCurveDataBlock() {};
		std::vector<uint32_t> frames;
		std::vector<T>        values;
	};

	class AnimationDataBlock
	{
	private:
		uint16_t num_frames = 1;
		float fps = 30;

		std::unordered_map<uint32_t, std::array<float, 4>> static_rotations;
		std::unordered_map<uint32_t, std::array<float, 3>> static_locations;
		std::unordered_map<uint32_t, std::array<float, 3>> static_scales;
		std::unordered_map<uint32_t, float>                static_shader_uniform_channels;

		std::unordered_map<uint32_t, AnimationCurveDataBlock<std::array<float, 4>>> animated_rotations;
		std::unordered_map<uint32_t, AnimationCurveDataBlock<std::array<float, 3>>> animated_locations;
		std::unordered_map<uint32_t, AnimationCurveDataBlock<std::array<float, 3>>> animated_scales;
		std::unordered_map<uint32_t, AnimationCurveDataBlock<float>> animated_shader_uniform_channels;

		std::list<uint32_t> unused_bone_ids;
		std::list<uint32_t> unused_shader_channel_ids;

	public:
		void setFPS(float fps) noexcept                   { this->fps = fps; };
		void setTotalFrames(uint16_t num_frames) noexcept { this->num_frames = num_frames; };
		const auto& getFPS() const noexcept                    { return this->fps; };
		const auto& getFrameCount() const noexcept             { return this->num_frames; };
		const auto& getStaticRotations() const noexcept        { return this->static_rotations; };
		const auto& getStaticLocations() const noexcept        { return this->static_locations; };
		const auto& getStaticScales() const noexcept           { return this->static_scales; };
		const auto& getStaticShaderChannels() const noexcept   { return this->static_shader_uniform_channels; };
		const auto& getAnimatedRotations() const noexcept      { return this->animated_rotations; };
		const auto& getAnimatedLocations() const noexcept      { return this->animated_locations; };
		const auto& getAnimatedScales() const noexcept         { return this->animated_scales; };
		const auto& getAnimatedShaderChannels() const noexcept { return this->animated_shader_uniform_channels; };
		const auto& getUnusedBoneIDs() const noexcept          { return this->unused_bone_ids; };
		const auto& getUnusedShaderChannelIDs() const noexcept { return this->unused_shader_channel_ids; };


		void addRotation(uint32_t bone_id, const std::array<float, 4>& value)
		{
			this->static_rotations[bone_id] = value;
		}
		void addLocation(uint32_t bone_id, const std::array<float, 3>& value)
		{
			this->static_locations[bone_id] = value;
		}
		void addScale(uint32_t bone_id, const std::array<float, 3>& value)
		{
			this->static_scales[bone_id] = value;
		}
		void addShaderChannel(uint32_t channel_id, float value)
		{
			this->static_shader_uniform_channels[channel_id] = value;
		}

		void addRotationKeyframe(uint32_t bone_id, uint32_t frame_idx, const std::array<float, 4>& value)
		{
			// Convert the static channel to an animated channel if it isn't already
			if (this->static_rotations.contains(bone_id))
			{
				this->animated_rotations.try_emplace(bone_id);
				if (frame_idx != 0)
				{
					this->animated_rotations[bone_id].frames.emplace_back(0);
					this->animated_rotations[bone_id].values.emplace_back(this->static_rotations[bone_id]);
				}
				this->static_rotations.erase(bone_id);
			}

			this->animated_rotations[bone_id].frames.emplace_back(frame_idx);
			this->animated_rotations[bone_id].values.emplace_back(value);
		}
		void addLocationKeyframe(uint32_t bone_id, uint32_t frame_idx, const std::array<float, 3>& value)
		{
			// Convert the static channel to an animated channel if it isn't already
			if (this->static_locations.contains(bone_id))
			{
				this->animated_locations.try_emplace(bone_id);
				if (frame_idx != 0)
				{
					this->animated_locations[bone_id].frames.emplace_back(0);
					this->animated_locations[bone_id].values.emplace_back(this->static_locations[bone_id]);
				}
				this->static_locations.erase(bone_id);
			}

			this->animated_locations[bone_id].frames.emplace_back(frame_idx);
			this->animated_locations[bone_id].values.emplace_back(value);
		}
		void addScaleKeyframe(uint32_t bone_id, uint32_t frame_idx, const std::array<float, 3>& value)
		{
			// Convert the static channel to an animated channel if it isn't already
			if (this->static_scales.contains(bone_id))
			{
				this->animated_scales.try_emplace(bone_id);
				if (frame_idx != 0)
				{
					this->animated_scales[bone_id].frames.emplace_back(0);
					this->animated_scales[bone_id].values.emplace_back(this->static_scales[bone_id]);
				}
				this->static_scales.erase(bone_id);
			}

			this->animated_scales[bone_id].frames.emplace_back(frame_idx);
			this->animated_scales[bone_id].values.emplace_back(value);
		}
		void addShaderChannelKeyframe(uint32_t bone_id, uint32_t frame_idx, const float& value)
		{
			// Convert the static channel to an animated channel if it isn't already
			if (this->static_shader_uniform_channels.contains(bone_id))
			{
				this->animated_shader_uniform_channels.try_emplace(bone_id);
				if (frame_idx != 0)
				{
					this->animated_shader_uniform_channels[bone_id].frames.emplace_back(0);
					this->animated_shader_uniform_channels[bone_id].values.emplace_back(this->static_shader_uniform_channels[bone_id]);
				}
				this->static_shader_uniform_channels.erase(bone_id);
			}

			this->animated_shader_uniform_channels[bone_id].frames.emplace_back(frame_idx);
			this->animated_shader_uniform_channels[bone_id].values.emplace_back(value);
		}

		void registerUnusedBoneIdxs(std::vector<char> usage_flags, uint16_t num_bones)
		{
			this->unused_bone_ids.clear();
			for (uint16_t i = 0; i < usage_flags.size(); ++i)
				if (usage_flags[i] == 0)
					this->unused_bone_ids.push_back(i);
		}

		void registerUnusedShaderChannelIdxs(std::vector<char> usage_flags, uint16_t num_channels)
		{
			this->unused_shader_channel_ids.clear();
			for (uint16_t i = 0; i < usage_flags.size(); ++i)
				if (usage_flags[i] == 0)
					this->unused_shader_channel_ids.push_back(i);
		}

	};
}