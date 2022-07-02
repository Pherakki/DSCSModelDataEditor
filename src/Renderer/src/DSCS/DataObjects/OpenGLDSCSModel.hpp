#pragma once

#include <memory>
#include <vector>
#include "OpenGLDSCSMesh.hpp"
#include "OpenGLDSCSMaterial.hpp"
#include "OpenGLDSCSTexture.hpp"
#include "SkeletonDataBlocks.hpp"
#include "AnimationDataBlocks.hpp"
#include "AnimationSampler.hpp"


namespace Rendering::DSCS::DataObjects
{
	class OpenGLDSCSModel
	{
	public:
		OpenGLDSCSModel() {};

		DataBlocks::Skeleton::SkeletonDataBlock skeleton;
		std::vector<std::shared_ptr<OpenGLDSCSMesh>> meshes;
		std::vector<std::shared_ptr<OpenGLDSCSMaterial>> materials;

		DataBlocks::Animation::AnimationDataBlock base_animation;
		std::vector<DataBlocks::Animation::AnimationDataBlock> animations;
		DataBlocks::Animation::BaseAnimationSampler base_anim_sampler;
		DataBlocks::Animation::AnimationSampler anim_sampler_a;
		DataBlocks::Animation::AnimationSampler anim_sampler_b;
		DataBlocks::Animation::AnimationSampler anim_sampler_c;

		#define safeCallSampler(sampler, method, ...) \
			if (sampler.isActive())                   \
				sampler. method (__VA_ARGS__);

		#define callOnSamplers(method, ...)                             \
			this->base_anim_sampler. method (__VA_ARGS__);              \
			safeCallSampler(this->anim_sampler_a, method, __VA_ARGS__); \
			safeCallSampler(this->anim_sampler_b, method, __VA_ARGS__)  \
			safeCallSampler(this->anim_sampler_c, method, __VA_ARGS__);

		void setSamplersFrame(float frame)
		{
			callOnSamplers(setFrame, frame);
		}

		void tickSamplers(float time)
		{
			callOnSamplers(tick, time)
		}

		void sampleSkeletalAnimation()
		{
			callOnSamplers(sampleCurrentFrame, this->skeleton.quat_buffer, this->skeleton.loc_buffer, this->skeleton.scale_buffer);
			this->skeleton.computeTransformBuffer();
		}

		void sampleShaderUniformAnimation(std::shared_ptr<Rendering::DSCS::DataObjects::OpenGLDSCSMaterial>& material, Rendering::DSCS::AnimationBuffer& animation_buffer)
		{
			callOnSamplers(sampleCurrentFrameUniforms, material->name_hash, animation_buffer.shader_uniform_buffer);
		}

		#undef callOnSamplers
		#undef safeCallSampler
	};
}
