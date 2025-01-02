#pragma once

#include "Casic/CasicGraphics.h"

namespace Casic
{
namespace GL
{
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