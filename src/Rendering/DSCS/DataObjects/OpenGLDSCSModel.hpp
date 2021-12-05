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
		std::vector<std::unique_ptr<OpenGLDSCSMesh>> meshes;
		std::vector<std::shared_ptr<OpenGLDSCSMaterial>> materials;

		DataBlocks::Animation::AnimationDataBlock base_animation;
		std::vector<DataBlocks::Animation::AnimationDataBlock> animations;
		DataBlocks::Animation::AnimationSampler anim_sampler;
		DataBlocks::Animation::BaseAnimationSampler base_anim_sampler;

	};
}
