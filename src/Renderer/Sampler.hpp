// Copyright 2016 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef sw_Sampler_hpp
#define sw_Sampler_hpp

#include "Main/Config.hpp"
#include "Renderer/Surface.hpp"

namespace sw
{
	struct Mipmap
	{
		const void *buffer[6];

		union
		{
			struct
			{
				int64_t uInt;
				int64_t vInt;
				int64_t wInt;
				int64_t uFrac;
				int64_t vFrac;
				int64_t wFrac;
			};

			struct
			{
				float4 fWidth;
				float4 fHeight;
				float4 fDepth;
			};
		};

		short uHalf[4];
		short vHalf[4];
		short wHalf[4];
		short width[4];
		short height[4];
		short depth[4];
		short onePitchP[4];
		int sliceP[2];
	};

	struct Texture
	{
		Mipmap mipmap[MIPMAP_LEVELS];

		float LOD;
		float4 widthHeightLOD;
		float4 widthLOD;
		float4 heightLOD;
		float4 depthLOD;

		word4 borderColor4[4];
		float4 borderColorF[4];
		float maxAnisotropy;
	};

	enum SamplerType
	{
		SAMPLER_PIXEL,
		SAMPLER_VERTEX
	};

	enum TextureType : unsigned int
	{
		TEXTURE_NULL,
		TEXTURE_2D,
		TEXTURE_CUBE,
		TEXTURE_3D,
		TEXTURE_2D_ARRAY,

		TEXTURE_LAST = TEXTURE_2D_ARRAY
	};

	enum FilterType : unsigned int
	{
		FILTER_POINT,
		FILTER_GATHER,
		FILTER_MIN_POINT_MAG_LINEAR,
		FILTER_MIN_LINEAR_MAG_POINT,
		FILTER_LINEAR,
		FILTER_ANISOTROPIC,

		FILTER_LAST = FILTER_ANISOTROPIC
	};

	enum MipmapType : unsigned int
	{
		MIPMAP_NONE,
		MIPMAP_POINT,
		MIPMAP_LINEAR,

		MIPMAP_LAST = MIPMAP_LINEAR
	};

	enum AddressingMode : unsigned int
	{
		ADDRESSING_WRAP,
		ADDRESSING_CLAMP,
		ADDRESSING_MIRROR,
		ADDRESSING_MIRRORONCE,
		ADDRESSING_BORDER,
		ADDRESSING_LAYER,

		ADDRESSING_LAST = ADDRESSING_LAYER
	};

	enum SwizzleType : unsigned int
	{
		SWIZZLE_RED,
		SWIZZLE_GREEN,
		SWIZZLE_BLUE,
		SWIZZLE_ALPHA,
		SWIZZLE_ZERO,
		SWIZZLE_ONE,

		SWIZZLE_LAST = SWIZZLE_ONE
	};

	class Sampler
	{
	public:
		struct State
		{
			State();

			TextureType textureType        : BITS(TEXTURE_LAST);
			Format textureFormat           : BITS(FORMAT_LAST);
			FilterType textureFilter       : BITS(FILTER_LAST);
			AddressingMode addressingModeU : BITS(ADDRESSING_LAST);
			AddressingMode addressingModeV : BITS(ADDRESSING_LAST);
			AddressingMode addressingModeW : BITS(ADDRESSING_LAST);
			MipmapType mipmapFilter        : BITS(FILTER_LAST);
			bool hasNPOTTexture	           : 1;
			bool sRGB                      : 1;
			SwizzleType swizzleR           : BITS(SWIZZLE_LAST);
			SwizzleType swizzleG           : BITS(SWIZZLE_LAST);
			SwizzleType swizzleB           : BITS(SWIZZLE_LAST);
			SwizzleType swizzleA           : BITS(SWIZZLE_LAST);

			#if PERF_PROFILE
			bool compressedFormat          : 1;
			#endif
		};

		Sampler();

		~Sampler();

		State samplerState() const;

		void setTextureLevel(int face, int level, Surface *surface, TextureType type);

		void setTextureFilter(FilterType textureFilter);
		void setMipmapFilter(MipmapType mipmapFilter);
		void setGatherEnable(bool enable);
		void setAddressingModeU(AddressingMode addressingMode);
		void setAddressingModeV(AddressingMode addressingMode);
		void setAddressingModeW(AddressingMode addressingMode);
		void setReadSRGB(bool sRGB);
		void setBorderColor(const Color<float> &borderColor);
		void setMaxAnisotropy(float maxAnisotropy);
		void setSwizzleR(SwizzleType swizzleR);
		void setSwizzleG(SwizzleType swizzleG);
		void setSwizzleB(SwizzleType swizzleB);
		void setSwizzleA(SwizzleType swizzleA);

		static void setFilterQuality(FilterType maximumFilterQuality);
		static void setMipmapQuality(MipmapType maximumFilterQuality);
		void setMipmapLOD(float lod);

		bool hasTexture() const;
		bool hasUnsignedTexture() const;
		bool hasCubeTexture() const;
		bool hasVolumeTexture() const;

		const Texture &getTextureData();

	private:
		MipmapType mipmapFilter() const;
		bool hasNPOTTexture() const;
		TextureType getTextureType() const;
		FilterType getTextureFilter() const;
		AddressingMode getAddressingModeU() const;
		AddressingMode getAddressingModeV() const;
		AddressingMode getAddressingModeW() const;

		Format externalTextureFormat;
		Format internalTextureFormat;
		TextureType textureType;

		FilterType textureFilter;
		AddressingMode addressingModeU;
		AddressingMode addressingModeV;
		AddressingMode addressingModeW;
		MipmapType mipmapFilterState;
		bool sRGB;
		bool gather;

		SwizzleType swizzleR;
		SwizzleType swizzleG;
		SwizzleType swizzleB;
		SwizzleType swizzleA;
		Texture texture;
		float exp2LOD;

		static FilterType maximumTextureFilterQuality;
		static MipmapType maximumMipmapFilterQuality;
	};
}

#endif   // sw_Sampler_hpp
