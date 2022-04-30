#pragma once


#include <algorithm>
#include "AnimationDataBlocks.hpp"
#include "SkeletonDataBlocks.hpp"
#include "../../../Utils/Vector.hpp"

namespace DataBlocks::Animation
{
	template<typename T, size_t N> 
	std::array<T, N> lerp(const std::array<T, N>& vec_1, const std::array<T, N>& vec_2, float t)
	{
		float first_interp_fac = 1.f - t;

		std::array<T, N> return_value;
		for (size_t i = 0; i < N; ++i)
			return_value[i] = vec_1[i] * first_interp_fac + vec_2[i] * t;

		return return_value;
	}

	float lerp(const float& float_1, const float& float_2, float t);
	std::array<float, 4> slerp(const std::array<float, 4>& quat_1, const std::array<float, 4>& quat_2, float t);

	template<typename T, T(*interp_func)(const T&, const T&, float)>
	T get_interpolated_framevalue(const AnimationCurveDataBlock<T>& animcurve, float current_frame)
	{
		auto next_element_itr = std::upper_bound(animcurve.frames.begin(), animcurve.frames.end(), current_frame);
		auto next_element_pos = (next_element_itr - animcurve.frames.begin());
		auto prev_element_pos = next_element_pos - 1;


		auto prev_frame = animcurve.frames[prev_element_pos];
		auto prev_value = animcurve.values[prev_element_pos];
		auto next_frame = prev_frame + 1;
		auto next_value = prev_value;
		if (next_element_pos != animcurve.frames.size())
		{
			next_frame = animcurve.frames[next_element_pos];
			next_value = animcurve.values[next_element_pos];
		}

		return (*interp_func)(prev_value, next_value, (current_frame - prev_frame) / (next_frame - prev_frame));
	}

	class AnimationSampler
	{
	public:
		void tick();
		void setAnim(AnimationDataBlock& anim) { this->anim = &anim; this->current_frame = 0.f; this->is_active = true; };
		void setSkel(Skeleton::SkeletonDataBlock& skel)
		{
			this->skel = &skel;
		};
		void deactivate() { this->anim = nullptr; this->is_active = false; }
		void sampleCurrentFrame(std::vector<std::array<float, 4>>& quats, std::vector<std::array<float, 3>>& locs, std::vector<std::array<float, 3>>& scales);
		void sampleCurrentFrameUniforms(DataBlocks::Skeleton::OBJHASH_t material_hash, std::array<float, 0xA0 * 16>& shader_channels);
	private:
		AnimationDataBlock* anim = nullptr;
		Skeleton::SkeletonDataBlock* skel = nullptr;
		bool is_active = false;
		float current_frame = 0.f;
		float target_spf = 1.f/30;
	};


	class BaseAnimationSampler
	{
	public:
		void tick();
		void setAnim(AnimationDataBlock& anim) { this->anim = &anim; };
		void setSkel(Skeleton::SkeletonDataBlock& skel) 
		{ 
			this->skel = &skel;
		};
		void sampleCurrentFrame(std::vector<std::array<float, 4>>& quats, std::vector<std::array<float, 3>>& locs, std::vector<std::array<float, 3>>& scales, std::array<float, 0xA0 * 16>& shader_channels);
		void sampleCurrentFrameUniforms(DataBlocks::Skeleton::OBJHASH_t material_hash, std::array<float, 0xA0 * 16>& shader_channels);
	private:
		AnimationDataBlock* anim = nullptr;
		Skeleton::SkeletonDataBlock* skel = nullptr;
		float current_frame = 0.f;
		float target_spf = 1.f / 30;
		std::vector<uint32_t> unused_bone_idxs;
		std::vector<uint32_t> unused_shader_channel_idxs;
	};


}