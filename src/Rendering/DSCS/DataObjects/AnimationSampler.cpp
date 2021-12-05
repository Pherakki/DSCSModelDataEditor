#include "AnimationSampler.hpp"

namespace DataBlocks::Animation
{
	float lerp(const float& float_1, const float& float_2, float t)
	{
		float first_interp_fac = 1.f - t;
		return float_1 * first_interp_fac + float_2 * t;
	}

	std::array<float, 4> slerp(const std::array<float, 4>& quat_1, const std::array<float, 4>& quat_2, float t)
	{
		float omega = acos(dot(quat_1, quat_2));
		if (omega == 0 || std::isnan(omega))
			return quat_1;
		float interp_fac = sin(t * omega) / sin(omega);

		return lerp(quat_1, { quat_2[0], quat_2[1], quat_2[2], quat_2[3] }, interp_fac);
	}


	void AnimationSampler::sampleNextFrame(std::vector<std::array<float, 4>>& quats, std::vector<std::array<float, 3>>& locs, std::vector<std::array<float, 3>>& scales, std::array<float, 0xA0 * 16>& shader_channels)
	{
		if (!is_active)
			return;

		for (auto& kv : this->anim->getStaticRotations())
			quats[kv.first] = mulQuats(quats[kv.first], kv.second);
		for (auto& kv : this->anim->getStaticLocations())
			locs[kv.first] = sumVects(locs[kv.first], kv.second);
		for (auto& kv : this->anim->getStaticScales())
			scales[kv.first] = sumVects(scales[kv.first], kv.second);
		for (auto& kv : this->anim->getStaticShaderChannels())
		{
			auto& id = this->skel->getShaderChannelDataBlocks().at(kv.first).shader_uniform_id;
			shader_channels[id] = kv.second;
		}

		// Wouldn't it be nice if the template parameters could just be automatically deduced by passing the function as a parameter?
		// Sigh...
		for (auto& kv : this->anim->getAnimatedRotations())
			quats[kv.first] = mulQuats(quats[kv.first], get_interpolated_framevalue<std::array<float, 4>, &slerp>(kv.second, this->current_frame));
		for (auto& kv : this->anim->getAnimatedLocations())
			locs[kv.first] = sumVects(locs[kv.first], get_interpolated_framevalue<std::array<float, 3>, &lerp>(kv.second, this->current_frame));
		for (auto& kv : this->anim->getAnimatedScales())
			scales[kv.first] = sumVects(scales[kv.first], get_interpolated_framevalue<std::array<float, 3>, &lerp>(kv.second, this->current_frame));
		for (auto& kv : this->anim->getAnimatedShaderChannels())
		{
			auto& id = this->skel->getShaderChannelDataBlocks().at(kv.first).shader_uniform_id;
			shader_channels[id] = get_interpolated_framevalue<float, &lerp>(kv.second, this->current_frame);
		}

		//quats[1];
		//locs[1];
		//scales[1];
		// Let's say that we want to sample 30 frames every second
		// We increment the current frame by 1/30 to to know where we should sample in the animation
		// But, the animation frames are stored as integers, not as times. So we multiply through by the animation's conversion factor
		this->current_frame += this->target_spf * anim->getFPS();
		if (this->current_frame > this->anim->getFrameCount())
			this->current_frame -= this->anim->getFrameCount();
	}


	void BaseAnimationSampler::sampleNextFrame(std::vector<std::array<float, 4>>& quats, std::vector<std::array<float, 3>>& locs, std::vector<std::array<float, 3>>& scales, std::array<float, 0xA0 * 16>& shader_channels)
	{
		for (auto& kv : this->skel->getBoneDataBlocks())
		{
			auto& idx = kv.first;
			auto& block = kv.second;
			quats[idx] = block.quat;//{ block.quat[1], block.quat[2], block.quat[3], block.quat[0] };
			locs[idx] = block.loc;
			scales[idx] = block.scale;
		}


		for (auto& kv : this->anim->getStaticRotations())
			quats[kv.first] = kv.second;
		for (auto& kv : this->anim->getStaticLocations())
			locs[kv.first] = kv.second;
		for (auto& kv : this->anim->getStaticScales())
			scales[kv.first] = kv.second;
		for (auto& kv : this->anim->getStaticShaderChannels())
		{
			auto& id = this->skel->getShaderChannelDataBlocks().at(kv.first).shader_uniform_id;
			shader_channels[id] = kv.second;
		}

		for (auto& kv : this->anim->getAnimatedRotations())
			quats[kv.first] = get_interpolated_framevalue<std::array<float, 4>, &slerp>(kv.second, this->current_frame);
		for (auto& kv : this->anim->getAnimatedLocations())
			locs[kv.first] = get_interpolated_framevalue<std::array<float, 3>, &lerp>(kv.second, this->current_frame);
		for (auto& kv : this->anim->getAnimatedScales())
			scales[kv.first] = get_interpolated_framevalue<std::array<float, 3>, &lerp>(kv.second, this->current_frame);
		for (auto& kv : this->anim->getAnimatedShaderChannels())
		{
			auto& id = this->skel->getShaderChannelDataBlocks().at(kv.first).shader_uniform_id;
			shader_channels[id] = get_interpolated_framevalue<float, &lerp>(kv.second, this->current_frame);
		}

		// Let's say that we want to sample 30 frames every second
		// We increment the current frame by 1/30 to to know where we should sample in the animation
		// But, the animation frames are stored as integers, not as times. So we multiply through by the animation's conversion factor
		this->current_frame += this->target_spf * anim->getFPS();
		if (this->current_frame > this->anim->getFrameCount())
			this->current_frame -= this->anim->getFrameCount();
	}

}