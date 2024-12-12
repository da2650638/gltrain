#pragma once

#include "GLGlobal.h"
#include "Casic/CasicGraphics.h"

namespace Casic
{
namespace GL
{
	struct RenderBuffer 
	{
		int BufferElements = 8192;
		int ElementVertice = 4;

		unsigned int VertexArray;

		unsigned int PositionBuffer;
		std::vector<float> PositionData;

		unsigned int TexCoordBuffer;
		std::vector<float> TexCoordData;

		unsigned int NormalBuffer;
		std::vector<float> NormalData;

		unsigned int ColorBuffer;
		std::vector<Graphics::Color> ColorData;

		unsigned int IndexBuffer;
		std::vector<unsigned int> IndexData;
	};

	struct DrawCall
	{
		int Mode;				//QUADS, TRIANGLES or LINES
		unsigned int Texture;
		int VertexCounter;
		int VertexAlignment;
	};

	struct RenderBatch
	{
		int CurrentBuffer = 0;
		int BufferCount = 5;
		int DrawCount = 256;
		int VertexCounter = 0;
		int DrawCounter = 0;

		float CurrentDepth = -1.0f;
		float DeptlDelta = 1.0f / 20000.0f;

		std::vector<RenderBuffer> Buffers;
		std::vector<DrawCall> Draws;
	};
}
}