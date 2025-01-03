#include "GLTexture.h"
#include "Casic/CasicUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <filesystem>

namespace Casic{
namespace GL {
	const char* GetPixelFormatName(int format)
	{
		switch (static_cast<PixelFormat>(format))
		{
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE: return "UNCOMPRESSED_GRAYSCALE";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA: return "UNCOMPRESSED_GRAY_ALPHA";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G6B5: return "UNCOMPRESSED_R5G6B5";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8: return "UNCOMPRESSED_R8G8B8";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1: return "UNCOMPRESSED_R5G5B5A1";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4: return "UNCOMPRESSED_R4G4B4A4";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8: return "UNCOMPRESSED_R8G8B8A8";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32: return "UNCOMPRESSED_R32";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32: return "UNCOMPRESSED_R32G32B32";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32: return "UNCOMPRESSED_R32G32B32A32";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16: return "UNCOMPRESSED_R16";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16: return "UNCOMPRESSED_R16G16B16";
		case PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16: return "UNCOMPRESSED_R16G16B16A16";

		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT1_RGB: return "COMPRESSED_DXT1_RGB";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT1_RGBA: return "COMPRESSED_DXT1_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT3_RGBA: return "COMPRESSED_DXT3_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_DXT5_RGBA: return "COMPRESSED_DXT5_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC1_RGB: return "COMPRESSED_ETC1_RGB";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC2_RGB: return "COMPRESSED_ETC2_RGB";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA: return "COMPRESSED_ETC2_EAC_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_PVRT_RGB: return "COMPRESSED_PVRT_RGB";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_PVRT_RGBA: return "COMPRESSED_PVRT_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA: return "COMPRESSED_ASTC_4x4_RGBA";
		case PixelFormat::GL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA: return "COMPRESSED_ASTC_8x8_RGBA";

		default: return "UNKNOWN_PIXEL_FORMAT";
		}
	}


	Graphics::Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize)
	{
		using Image = typename Graphics::Image;
		Image image{ 0 };

		// TODO: 这里只是实现了少数集中非常常见的格式，还有更多图像格式急需提供支持
		if ((false)
#if defined(SUPPORT_FILEFORMAT_PNG)
			|| !std::strcmp(fileType, ".png") || !std::strcmp(fileType, ".PNG")
#endif
#if defined(SUPPORT_FILEFORMAT_JPG)
			|| !std::strcmp(fileType, ".jpg") || !std::strcmp(fileType, ".JPG")
#endif
#if defined(SUPPORT_FILEFORMAT_GIF)
			|| !std::strcmp(fileType, ".gif") || !std::strcmp(fileType, ".GIF")
#endif
			)
		{
#if defined(STBI_REQUIRED)
			// NOTE: Using stb_image to load images (Supports multiple image formats)
			int comp = 0;
			image.data = stbi_load_from_memory(fileData, dataSize, &image.width, &image.height, &comp, 0);
			
			if (comp == 1)
			{
				image.format = static_cast<int>(PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
			}
			else if (comp == 2)
			{
				image.format = static_cast<int>(PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA);
			}
			else if (comp == 3)
			{
				image.format = static_cast<int>(PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8);
			}
			else if (comp == 4)
			{
				image.format = static_cast<int>(PixelFormat::GL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			}
			image.mipmaps = 1;
#endif
		}
		else
		{
			SimpleLogger::GetInstance().Error("IMAGE: Data format {} not supported", fileType);
		}

		if (image.data)
		{
			SimpleLogger::GetInstance().Info("IMAGE: Data loaded successfully ({}x{} | {} | {} mipmaps)", image.width, image.height, GetPixelFormatName(image.format), image.mipmaps);
		}
		else
		{
			SimpleLogger::GetInstance().Error("IMAGE: Failed to load image data");
		}

		return image;
	}

	Graphics::Image LoadImageStruct(const char* fileName)
	{
		using Image = typename Graphics::Image;
		Image image{ 0 };
		auto [fileData, dataSize] = Util::LoadFileData(fileName);
		if (fileData && dataSize > 0)
		{
			std::string fileType = std::filesystem::path(fileName).extension().string();
			image = LoadImageFromMemory(fileType.c_str(), fileData, dataSize);
			Util::UnloadFileData(fileData);
			return image;
		}
		else
		{
			SimpleLogger::GetInstance().Error("Image: Failed to load file data, fileName: [{}]", fileName);
			return image;
		}
	}

	void UnloadImageStruct(Graphics::Image image)
	{
		stbi_image_free(image.data);
	}

	Texture2D LoadTexture(const char* fileName)
	{
		using Image = typename Graphics::Image;
		Image image = LoadImageStruct(fileName);
		Texture2D texture{ 0 };
		// TODO: 未完待续

		if (image.data)
		{
			UnloadImageStruct(image);
		}
		
		return texture;
	}

}
}