#pragma once

#include <glad/glad.h>

#include <vector>

#include "GLGlobalState.h"

struct GLVertexBuffer {
public:
	GLVertexBuffer() {}
	~GLVertexBuffer() {}

	void Init(int elementCount);
	void Shutdown();
public:
	bool m_Initialize = false;

	unsigned int m_Vao = 0;

	unsigned int m_PosVbo = 0;
	std::vector<float> m_Positions;

	unsigned int m_ColVbo = 0;
	std::vector<float> m_Colors;

	unsigned int m_IdxVbo = 0;
	std::vector<unsigned int> m_Indices;
};