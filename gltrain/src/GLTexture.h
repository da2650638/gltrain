#pragma once

#include "Casic/CasicGraphics.h"
#include "GLGlobal.h"

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

    using Texture2D = typename Texture;
    using GLTexture = typename Texture2D;

    static inline std::tuple<unsigned int, unsigned int, unsigned int> GetGLTextureFormat(PixelFormat format)
    {
        switch (format)
        {
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
            return { GL_R8, GL_RED, GL_UNSIGNED_BYTE };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
            return { GL_RG8, GL_RG, GL_UNSIGNED_BYTE };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G6B5:
            return { GL_RGB565, GL_RGB, GL_UNSIGNED_SHORT_5_6_5 };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8:
            return { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
            return { GL_RGB5_A1, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1 };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
            return { GL_RGBA4, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4 };
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
            return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE };
            // TODO: 剩下的格式得先判断当前OpenGL是否支持，暂时先不做判断
        default:
            return { 0,0,0 };
        }
    }

    /**
     * Calculates the size of pixel data for a given image format and dimensions.
     *
     * @param width (int): The width of the image in pixels.
     * @param height (int): The height of the image in pixels.
     * @param format (PixelFormat): The format of the pixel data (uncompressed or compressed).
     *
     * @return (int): The size of the image data in bytes.
     *
     * @note:
     * - For uncompressed formats, the size is calculated as width * height * bytes_per_pixel.
     * - For compressed formats, the size is determined based on the block compression scheme.
     * - If the format is unsupported, the function returns 0.
     */
    static inline int GetPixelDataSize(int width, int height, PixelFormat format)
    {
        int bits_per_pixel = 0;

        switch (format)
        {
            // Uncompressed formats
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
            bits_per_pixel = 8; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
            bits_per_pixel = 16; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G6B5:
            bits_per_pixel = 16; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8:
            bits_per_pixel = 24; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1:
            bits_per_pixel = 16; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4:
            bits_per_pixel = 16; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
            bits_per_pixel = 32; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32:
            bits_per_pixel = 32; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32:
            bits_per_pixel = 96; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
            bits_per_pixel = 128; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16:
            bits_per_pixel = 16; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16:
            bits_per_pixel = 48; break;
        case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16:
            bits_per_pixel = 64; break;

            // Compressed formats (approximate size based on bits per pixel)
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT1_RGB:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT1_RGBA:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC1_RGB:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC2_RGB:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_PVRT_RGB:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_PVRT_RGBA:
            bits_per_pixel = 4; break;
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT3_RGBA:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT5_RGBA:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA:
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA:
            bits_per_pixel = 8; break;
        case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA:
            bits_per_pixel = 2; break;

        default:
            bits_per_pixel = 0; // Unsupported format
            break;
        }

        // TODO: 这里需要了解一下
        // For compressed formats, size is calculated differently
        if (format >= PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT1_RGB &&
            format <= PixelFormat::GL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA) {
            int block_size = (bits_per_pixel == 4) ? 8 : 16; // Block size in bytes (DXT1 = 8, others = 16)
            int block_width = 4;
            int block_height = 4;

            int blocks_x = (width + block_width - 1) / block_width;
            int blocks_y = (height + block_height - 1) / block_height;

            return blocks_x * blocks_y * block_size;
        }

        // For uncompressed formats
        double bytes_per_pixel = static_cast<double>(bits_per_pixel) / 8.0;
        int dataSize = static_cast<int>(bytes_per_pixel * width * height);

        return dataSize;
    }

    static inline const char* GetPixelFormatName(int format);

    GLTRAIN_API Graphics::Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize);
    GLTRAIN_API Graphics::Image LoadImageStruct(const char* fileName);
    GLTRAIN_API void UnloadImageStruct(Graphics::Image image);
    
    GLTRAIN_API Texture2D LoadTexture(const char* fileName);
    GLTRAIN_API void UnloadTexture(Texture2D texture);

    GLTRAIN_API Texture2D LoadTextureFromImage(Graphics::Image image);
    
    GLTRAIN_API unsigned int GLLoadTexture(const void* data, int width, int height, int format, int mipmapCount);
    GLTRAIN_API void GLUnloadTexture(unsigned int id);
}
}