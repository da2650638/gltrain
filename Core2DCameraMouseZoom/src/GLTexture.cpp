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
			texture = LoadTextureFromImage(image);
			UnloadImageStruct(image);
		}
		
		return texture;
	}

	void UnloadTexture(Texture2D texture)
	{
		if (texture.id > 0)
		{
			GLUnloadTexture(texture.id); 

			SimpleLogger::GetInstance().Info("TEXTURE: [ID {}] Unloaded texture data from VRAM (GPU)", texture.id);
		}

	}

	Texture2D LoadTextureFromImage(Graphics::Image image)
	{
		Texture2D texture{ 0 };
		if (image.width != 0 && image.height != 0)
		{
			texture.id = GLLoadTexture(image.data, image.width, image.height, image.format, image.mipmaps);
		}
		else
		{
			SimpleLogger::GetInstance().Error("IMAGE: Data is not valid to load texture");
		}
		if (texture.id > 0)
		{
			SimpleLogger::GetInstance().Info("TEXTURE: Load texture [{}] successfully.", texture.id);
			texture.width = image.width;
			texture.height = image.height;
			texture.format = image.format;
			texture.mipmaps = image.mipmaps;
		}
		else
		{
			SimpleLogger::GetInstance().Error("TEXTURE: Load from image failed.");
		}
		return texture;
	}

	unsigned int GLLoadTexture(const void* data, int width, int height, int format, int mipmapCount)
	{
		unsigned int id = 0;
		// TODO: 对当前OpenGL的能力进行判断，如果是某些特殊格式的图片则无法支持，直接返回id为0的情况。;
		auto [glInternalFormat, glFormat, glType] = GetGLTextureFormat(static_cast<PixelFormat>(format));
		if (glInternalFormat == 0)
		{
			SimpleLogger::GetInstance().Error("TEXTURE: Unsupported texture pixel format: [{}]", format);
			return id;
		}
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, mipmapCount, glInternalFormat, width, height);
		int mipmapOffset = 0, mipmapWidth = width, mipmapHeight = height;
		unsigned char* dataPtr = nullptr;
		if (data) dataPtr = (unsigned char*)data;
		for (int i = 0; i < mipmapCount; i++)
		{
			int dataSize = GetPixelDataSize(mipmapWidth, mipmapHeight, static_cast<PixelFormat>(format));
			SimpleLogger::GetInstance().Info("TEXTURE: Load mipmap level {} ({} x {}), size: {}, offset: {}", i, mipmapWidth, mipmapHeight, dataSize, mipmapOffset);

			// TODO: 某些特殊的压缩的图像格式需要添加代码做特别处理

			glTextureSubImage2D(id, i, 0, 0, mipmapWidth, mipmapHeight, glFormat, glType, dataPtr);

			mipmapWidth /= 2;
			mipmapHeight /= 2;
			if(data) dataPtr += dataSize;
			mipmapOffset += dataSize;
			if (mipmapWidth < 1) mipmapWidth = 1;
			if (mipmapHeight < 1) mipmapHeight = 1;
		}
		glTexParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (mipmapCount > 1)
		{
			glTexParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		if (id > 0) SimpleLogger::GetInstance().Info("TEXTURE: id: [{}] load successfully, mipmaps: {}, size: {}x{}, format:{}", id, mipmapCount, width, height, GetPixelFormatName(format));
		else SimpleLogger::GetInstance().Info("TEXTURE: Failed to load texture.");

		return id;
	}

	void GLUnloadTexture(unsigned int id)
	{
		glDeleteTextures(1, &id);
	}

}
}