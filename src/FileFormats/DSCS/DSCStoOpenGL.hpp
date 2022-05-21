#pragma once


#include "GeomFile/GeomReadWrite.hpp"
#include "NameFile/NameReadWrite.hpp"
#include "SkelFile/SkelReadWrite.hpp"
#include "AnimFile/AnimReadWrite.hpp"
#include "../Textures/DDS.hpp"
#include "../../UI/TextureLibrary.hpp"
#include "../../Rendering/DSCS/Renderer.hpp"
#include "../../Rendering/DSCS/DataObjects/OpenGLDSCSModel.hpp"
#include "../../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
#include "../../Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
#include "../../Utils/OpenGL.hpp"
#include "../../Utils/Matrix.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <unordered_map>


// Init used textures
uint16_t initTexture(uint16_t param_type,
	const std::array<uint16_t, 8>& data,
	const std::filesystem::path& img_path,
	const std::vector<std::array<char, 32>>& texture_names,
	TextureLibrary&texture_library);


namespace FileFormats::DSCS
{
	template<typename T, typename bitvector_T, void(DataBlocks::Animation::AnimationDataBlock::*func)(uint32_t, uint32_t, const T&)>
	void addElementKeyframes(DataBlocks::Animation::AnimationDataBlock& anim, int32_t frames_passed, uint32_t num_contained_frames, uint32_t subframes_passed,
		                     const std::vector<bitvector_T>& frame_bitvector, unsigned long& current_frame_bitvector_piece, uint32_t& current_frame_bitvector_piece_idx,
		                     const std::vector<uint16_t>& element_indices, const std::vector<T>& initial_elements, const std::vector<T>& keyframed_elements)
	{
		uint32_t current_element = 0;
		constexpr uint8_t bitvector_entries_size = sizeof(std::remove_reference_t<decltype(frame_bitvector)>::value_type) * 8;
		for (uint32_t j = 0; j < element_indices.size(); ++j)
		{
			auto& curr_bone = element_indices[j];
			(anim.*func)(curr_bone, frames_passed, initial_elements[j]);

			// Read in keyframed bits
			uint32_t current_frame = 0;
			unsigned long skipped_frames;
			unsigned long skipped_frames_reversed;
			unsigned long frame_increment;
			while (current_frame <= num_contained_frames)
			{
				// Find next '1' in the piece of the bitvector we're looking at
				bool found_frame = _BitScanReverse(&skipped_frames_reversed, current_frame_bitvector_piece);
				if (found_frame)
				{
					skipped_frames = (bitvector_entries_size - 1) - skipped_frames_reversed;
					frame_increment = (skipped_frames + 1);
					// Calculate what frame it corresponds to, exit the loop if it should belong to whatever comes next
					current_frame = (current_frame_bitvector_piece_idx * bitvector_entries_size) - ((j + subframes_passed) * num_contained_frames) + frame_increment;
					if (current_frame > num_contained_frames)
						break;

					// Add the keyframe to the animation, on whatever channel *func yeets it into
					(anim.*func)(curr_bone, current_frame + frames_passed, keyframed_elements[current_element]);

					++current_element;
					// Delete the '1' from the bitvector copy, so we don't run into it again
					current_frame_bitvector_piece ^= (1 << skipped_frames_reversed);
				}
				else
				{
					++current_frame_bitvector_piece_idx;
					current_frame = (current_frame_bitvector_piece_idx * bitvector_entries_size) - ((j + subframes_passed) * num_contained_frames);
					if (current_frame_bitvector_piece_idx >= frame_bitvector.size())
						break;
					current_frame_bitvector_piece = frame_bitvector[current_frame_bitvector_piece_idx];
				}
			}
		}
		auto& a = anim;
		if constexpr (!std::is_same<T, float>::value)
			assert(current_element == keyframed_elements.size());
	}

	template<bool is_base>
	DataBlocks::Animation::AnimationDataBlock& chooseAnimationReference(Rendering::DSCS::DataObjects::OpenGLDSCSModel& model)
	{
		if constexpr (is_base)
		{
			model.base_animation = DataBlocks::Animation::AnimationDataBlock();
			return model.base_animation;
		}
		else
		{
			// Add a new animation to the animation vector in-place, and get a reference to it so we can modify it
			model.animations.emplace_back(DataBlocks::Animation::AnimationDataBlock());
			return model.animations.back();
		}
	}

	template<bool is_base>
	void loadAnimation(Rendering::DSCS::DataObjects::OpenGLDSCSModel& model, uint16_t num_shader_channels, const std::filesystem::path& filepath)
	{
		auto anim_file = AnimFile::AnimReadWrite(num_shader_channels);
		anim_file.read(filepath.string());

		DataBlocks::Animation::AnimationDataBlock& anim = chooseAnimationReference<is_base>(model);

		anim.setFPS(anim_file.getFPS());
		anim.setTotalFrames(anim_file.getTotalFrames());

		// Do the static frames
		auto& static_rotation_idxs = anim_file.getStaticRotationIdxs();
		auto& static_rotation_vals = anim_file.getStaticRotationValues();
		for (uint32_t i = 0; i < anim_file.getStaticRotationsCount(); ++i)
			anim.addRotation(static_rotation_idxs[i], static_rotation_vals[i]);

		auto& static_location_idxs = anim_file.getStaticLocationIdxs();
		auto& static_location_vals = anim_file.getStaticLocationValues();
		for (uint32_t i = 0; i < anim_file.getStaticLocationsCount(); ++i)
			anim.addLocation(static_location_idxs[i], static_location_vals[i]);

		auto& static_scale_idxs = anim_file.getStaticScaleIdxs();
		auto& static_scale_vals = anim_file.getStaticScaleValues();
		for (uint32_t i = 0; i < anim_file.getStaticScalesCount(); ++i)
			anim.addScale(static_scale_idxs[i], static_scale_vals[i]);

		auto& static_shader_channel_idxs = anim_file.getStaticShaderChannelIdxs();
		auto& static_shader_channel_vals = anim_file.getStaticShaderChannelValues();
		for (uint32_t i = 0; i < anim_file.getStaticShaderChannelsCount(); ++i)
			anim.addShaderChannel(static_shader_channel_idxs[i], static_shader_channel_vals[i]);

		// Now do the animated ones
		auto& keyframe_counts     = anim_file.getKeyframeChunkElementCounts();
		auto& keyframe_chunks     = anim_file.getKeyframeChunks();
		auto& num_keyframe_chunks = anim_file.getNumKeyframeChunks();
		auto& animated_rotation_idxs       = anim_file.getAnimatedRotationIdxs();
		auto& animated_location_idxs       = anim_file.getAnimatedLocationIdxs();
		auto& animated_scale_idxs          = anim_file.getAnimatedScaleIdxs();
		auto& animated_shader_channel_idxs = anim_file.getAnimatedShaderChannelIdxs();

		for (uint32_t i = 0; i < anim_file.getAnimatedRotationsCount(); ++i)
			anim.addRotation(animated_rotation_idxs[i], {});
		for (uint32_t i = 0; i < anim_file.getAnimatedLocationsCount(); ++i)
			anim.addLocation(animated_location_idxs[i], {});
		for (uint32_t i = 0; i < anim_file.getAnimatedScalesCount(); ++i)
			anim.addScale(animated_scale_idxs[i], {});
		for (uint32_t i = 0; i < anim_file.getAnimatedShaderChannelsCount(); ++i)
			anim.addShaderChannel(animated_shader_channel_idxs[i], {});

		size_t frames_passed = 0;
		for (auto i = decltype(num_keyframe_chunks){0}; i < num_keyframe_chunks; ++i)
		{
			auto& current_chunk = keyframe_chunks[i];
			auto& num_contained_frames = keyframe_counts[i].increment;

			auto& frame_bitvector = current_chunk.getFrameBitvector();
			uint32_t current_frame_bitvector_piece_idx = 0;
			uint8_t position_in_bitvector_piece = 0;

			unsigned long current_frame_bitvector_piece;
			if (!frame_bitvector.size())
				current_frame_bitvector_piece = 0;
			else
				current_frame_bitvector_piece = frame_bitvector[current_frame_bitvector_piece_idx];

			uint32_t subchunks_passed = 0;
			// Enter the rotations
			auto& initial_rotations = current_chunk.getInitialRotations();
			auto& keyframed_rotations = current_chunk.getKeyframedRotations();
			addElementKeyframes<std::array<float, 4>, uint8_t, &DataBlocks::Animation::AnimationDataBlock::addRotationKeyframe>
			(anim, frames_passed, num_contained_frames, subchunks_passed, frame_bitvector, current_frame_bitvector_piece, current_frame_bitvector_piece_idx, animated_rotation_idxs, initial_rotations, keyframed_rotations);
			subchunks_passed += animated_rotation_idxs.size();

			// Enter the locations
			auto& initial_locations = current_chunk.getInitialLocations();
			auto& keyframed_locations = current_chunk.getKeyframedLocations();
			addElementKeyframes<std::array<float, 3>, uint8_t, &DataBlocks::Animation::AnimationDataBlock::addLocationKeyframe>
			(anim, frames_passed, num_contained_frames, subchunks_passed, frame_bitvector, current_frame_bitvector_piece, current_frame_bitvector_piece_idx, animated_location_idxs, initial_locations, keyframed_locations);
			subchunks_passed += animated_location_idxs.size();

			// Enter the scales
			auto& initial_scales = current_chunk.getInitialScales();
			auto& keyframed_scales = current_chunk.getKeyframedScales();
			addElementKeyframes<std::array<float, 3>, uint8_t, &DataBlocks::Animation::AnimationDataBlock::addScaleKeyframe>
			(anim, frames_passed, num_contained_frames, subchunks_passed, frame_bitvector, current_frame_bitvector_piece, current_frame_bitvector_piece_idx, animated_scale_idxs, initial_scales, keyframed_scales);
			subchunks_passed += animated_scale_idxs.size();

			// Enter the rotations
			auto& initial_shader_channel_values = current_chunk.getInitialShaderChannelValues();
			auto& keyframed_shader_channel_values = current_chunk.getKeyframedShaderChannelValues();
			addElementKeyframes<float, uint8_t, &DataBlocks::Animation::AnimationDataBlock::addShaderChannelKeyframe>
			(anim, frames_passed, num_contained_frames, subchunks_passed, frame_bitvector, current_frame_bitvector_piece, current_frame_bitvector_piece_idx, animated_shader_channel_idxs, initial_shader_channel_values, keyframed_shader_channel_values);
			subchunks_passed += animated_shader_channel_idxs.size();

			// +1 because the increment doesn't count the initial frame
			frames_passed += num_contained_frames + 1;
		}
		
		anim.fixQuaternions();
		anim.registerUnusedBoneIdxs(anim_file.getUnusedBoneFlags(), anim_file.getNumBones());
		anim.registerUnusedShaderChannelIdxs(anim_file.getUnusedShaderChannelFlags(), num_shader_channels);
	}


	// Should convert all data structures in here to use the "Data Blocks" prescription
	Rendering::DSCS::DataObjects::OpenGLDSCSModel DSCStoOpenGL(const std::filesystem::path& filepath,
															   std::unique_ptr<Rendering::ShaderBackends::cgGLShaderBackend>& shader_backend,
														       const std::array<float*, 0xA0>& uniform_dispatch_buffer,
		                                                       TextureLibrary& texture_library,
		                                                       std::unordered_map<std::string, std::shared_ptr<Rendering::ShaderObjects::cgGLShaderObject>>& shader_library);
}