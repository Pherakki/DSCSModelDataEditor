#pragma once

#include <array>

#include "../../Utils/SubarrayView.hpp"

namespace Rendering::DSCS
{
	class AnimationBuffer
	{
	public:
		std::array<float, 4*56*3> matrix_palette_buffer;
		std::array<float, 0xA0 * 0x10>  shader_uniform_buffer;
		std::array<float*, 0xA0>      uniform_dispatch_buffer;

		std::unique_ptr<SubarrayView<float,  16>> ViewProj;
		std::unique_ptr<SubarrayView<float,  16>> World;
		std::unique_ptr<SubarrayView<float,  16>> View;
		std::unique_ptr<SubarrayView<float,  16>> ViewInverse;
		std::unique_ptr<SubarrayView<float,  16>> WorldInverse;
		std::unique_ptr<SubarrayView<float, 672>> MatrixPalette;
		std::unique_ptr<SubarrayView<float,   1>> SystemAlpha;
		std::unique_ptr<SubarrayView<float,   2>> ViewportPixelSize;
		std::unique_ptr<SubarrayView<float,   4>> DirLamp01Color;
		std::unique_ptr<SubarrayView<float,   2>> ColorSamplerScale;
		std::unique_ptr<SubarrayView<float,   3>> DirLamp01Dir;
		std::unique_ptr<SubarrayView<float,   4>> DirLamp02Color;
		std::unique_ptr<SubarrayView<float,   1>> bloomLimit;
		std::unique_ptr<SubarrayView<float,   3>> DirLamp02Dir;
		std::unique_ptr<SubarrayView<float,   4>> DirLamp03Color;
		std::unique_ptr<SubarrayView<float,   2>> blurOffset;
		std::unique_ptr<SubarrayView<float,   3>> DirLamp03Dir;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp01Color;
		std::unique_ptr<SubarrayView<float,   1>> ShadowDensity;
		std::unique_ptr<SubarrayView<float,   1>> PointLamp01Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp01Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp02Color;
		std::unique_ptr<SubarrayView<float,   1>> bloomScale;
		std::unique_ptr<SubarrayView<float,   1>> PointLamp02Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp02Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp03Color;
												
		std::unique_ptr<SubarrayView<float,   1>> PointLamp03Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp03Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp04Color;
												
		std::unique_ptr<SubarrayView<float,   1>> PointLamp04Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp04Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp05Color;
												
		std::unique_ptr<SubarrayView<float,   1>> PointLamp05Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp05Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp06Color;
		std::unique_ptr<SubarrayView<float,   4>> dofParams;
		std::unique_ptr<SubarrayView<float,   1>> PointLamp06Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp06Pos;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp07Color;
		std::unique_ptr<SubarrayView<float,   4>> dofOptions;
		std::unique_ptr<SubarrayView<float,   1>> PointLamp07Range;
		std::unique_ptr<SubarrayView<float,   4>> PointLamp07Pos;
		std::unique_ptr<SubarrayView<float,   3>> AmbientColor;
		std::unique_ptr<SubarrayView<float,   3>> SkyDir;
		std::unique_ptr<SubarrayView<float,   1>> dofGrayScale;
		std::unique_ptr<SubarrayView<float,   3>> GroundColor;
												
		std::unique_ptr<SubarrayView<float,  16>> LightViewProj;
		std::unique_ptr<SubarrayView<float,   3>> CameraPosition;
		std::unique_ptr<SubarrayView<float,   1>> Time;
												
		std::unique_ptr<SubarrayView<float,   3>> FogParams;
		std::unique_ptr<SubarrayView<float,   1>> FogNear;
		std::unique_ptr<SubarrayView<float,   1>> FogFar;
		std::unique_ptr<SubarrayView<float,   1>> FogAlpha;
		std::unique_ptr<SubarrayView<float,   3>> FogColor;
												
		std::unique_ptr<SubarrayView<float,   3>> ViewOffset;
		std::unique_ptr<SubarrayView<float,   4>> ScreenVertexSize;
		std::unique_ptr<SubarrayView<float,   4>> ColorTextureSize;

		AnimationBuffer()
		{
			for (int i = 0; i < this->uniform_dispatch_buffer.size(); ++i)
				this->uniform_dispatch_buffer[i] = &this->shader_uniform_buffer[0x10 * i];
			for (int i = 0; i < this->shader_uniform_buffer.size(); ++i)
				this->shader_uniform_buffer[i] = 0.;
			for (int i = 0; i < this->matrix_palette_buffer.size(); ++i)
				this->matrix_palette_buffer[i] = 0.;
			for (int i = 0; i < this->matrix_palette_buffer.size(); i += 12)
				this->matrix_palette_buffer[i] = 1.;
			for (int i = 5; i < this->matrix_palette_buffer.size(); i += 12)
				this->matrix_palette_buffer[i] = 1.;
			for (int i = 10; i < this->matrix_palette_buffer.size(); i += 12)
				this->matrix_palette_buffer[i] = 1.;
			this->uniform_dispatch_buffer[0x06] = &this->matrix_palette_buffer[0];

			this->ViewProj          = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x01]);
			this->World             = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x02]);
			this->View              = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x03]);
			this->ViewInverse       = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x04]);
			this->WorldInverse      = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x05]);
			this->MatrixPalette     = std::make_unique<SubarrayView<float, 672>>(*this->uniform_dispatch_buffer[0x06]);
			this->SystemAlpha       = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x07]);
			this->ViewportPixelSize = std::make_unique<SubarrayView<float, 2>>(*this->uniform_dispatch_buffer[0x08]);
			this->DirLamp01Color    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x09]);
			this->ColorSamplerScale = std::make_unique<SubarrayView<float, 2>>(*this->uniform_dispatch_buffer[0x0A]);
			this->DirLamp01Dir      = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x0B]);
			this->DirLamp02Color    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x0C]);
			this->bloomLimit        = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x0D]);
			this->DirLamp02Dir      = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x0E]);
			this->DirLamp03Color    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x0F]);
			this->blurOffset        = std::make_unique<SubarrayView<float, 2>>(*this->uniform_dispatch_buffer[0x10]);
			this->DirLamp03Dir      = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x11]);
			this->PointLamp01Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x12]);
			this->ShadowDensity     = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x13]);
			this->PointLamp01Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x14]);
			this->PointLamp01Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x15]);
			this->PointLamp02Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x16]);
			this->bloomScale        = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x17]);
			this->PointLamp02Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x18]);
			this->PointLamp02Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x19]);
			this->PointLamp03Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x1A]);
								    
			this->PointLamp03Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x1C]);
			this->PointLamp03Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x1D]);
			this->PointLamp04Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x1E]);
								    
			this->PointLamp04Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x20]);
			this->PointLamp04Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x21]);
			this->PointLamp05Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x22]);
								    
			this->PointLamp05Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x24]);
			this->PointLamp05Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x25]);
			this->PointLamp06Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x26]);
								    
			this->dofParams         = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x27]);
			this->PointLamp06Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x28]);
			this->PointLamp06Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x29]);
			this->PointLamp07Color  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x2A]);
			this->dofOptions        = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x2B]);
			this->PointLamp07Range  = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x2C]);
			this->PointLamp07Pos    = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x2D]);
			this->AmbientColor      = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x2E]);
			this->SkyDir            = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x2F]);
			this->dofGrayScale      = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x30]);
			this->GroundColor       = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x31]);
								    
			this->LightViewProj     = std::make_unique<SubarrayView<float, 16>>(*this->uniform_dispatch_buffer[0x52]);
			this->CameraPosition    = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x53]);
			this->Time              = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x54]);
								    
			this->FogParams         = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x65]);
			this->FogNear           = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x66]);
			this->FogFar            = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x67]);
			this->FogAlpha          = std::make_unique<SubarrayView<float, 1>>(*this->uniform_dispatch_buffer[0x68]);
			this->FogColor          = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x69]);
								    
			this->ViewOffset        = std::make_unique<SubarrayView<float, 3>>(*this->uniform_dispatch_buffer[0x9D]);
			this->ScreenVertexSize  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x9E]);
			this->ColorTextureSize  = std::make_unique<SubarrayView<float, 4>>(*this->uniform_dispatch_buffer[0x9F]);
		}
	};

}
