#pragma once

#include "Casic/CasicGraphics.h"

namespace Casic
{
namespace GL
{
	enum class PixelFormat {
		// NOTES: bits per pixel = bpp
		GL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1,		// 8  bpp(1 channel)-1 byte per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,			// 16 bpp(2 channels)-2 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R5G6B5,				// 16 bpp(3 channels)-2 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8,				// 24 bpp(3 channels)-3 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,			// 16 bpp(4 channels)-2 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,			// 16 bpp(4 channels)-2 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,			// 32 bpp(4 channels)-4 bytes per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R32,				// 32 bpp(1 channel)-4 bytes per pixel-1 float per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32,			// 96 bpp(3 channels)-12 bytes per pixel-3 floats per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,		// 128 bpp(4 channels)-16 bytes per pixel-4 floats per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R16,				// 16 bpp(1 channel)-2 bytes per pixel-half float per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16,			// 48 bpp(3 channels)-6 bytes per pixel-3 half floats per pixel
		GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,		// 64 bpp(4 channels)-8 bytes per pixel-4 half floats per pixel

		GL_PIXELFORMAT_COMPRESSED_DXT1_RGB,				// 4 bpp (no alpha)
		GL_PIXELFORMAT_COMPRESSED_DXT1_RGBA,			// 4 bpp (1 bit alpha)
		GL_PIXELFORMAT_COMPRESSED_DXT3_RGBA,			// 8 bpp
		GL_PIXELFORMAT_COMPRESSED_DXT5_RGBA,			// 8 bpp
		GL_PIXELFORMAT_COMPRESSED_ETC1_RGB,				// 4 bpp
		GL_PIXELFORMAT_COMPRESSED_ETC2_RGB,				// 4 bpp
		GL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,		// 8 bpp
		GL_PIXELFORMAT_COMPRESSED_PVRT_RGB,				// 4 bpp
		GL_PIXELFORMAT_COMPRESSED_PVRT_RGBA,			// 4 bpp
		GL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,		// 8 bpp
		GL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA			// 2 bpp
	};

	typedef struct Texture {
		unsigned int id;
		int width;
		int height;
		int mipmaps;
		int format;
	}Texture;

	using Texture2D = Texture;
}
}