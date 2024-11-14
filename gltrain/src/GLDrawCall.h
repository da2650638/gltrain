#pragma once

#include <glad/glad.h>

#define LINES GL_LINES
#define TRIANGLES GL_TRIANGLES
#define QUADS GL_QUADS

struct GLDrawCall {
	int Mode;
	int TextureID;
	int VertexAlignment;
	int VertexCount;
};