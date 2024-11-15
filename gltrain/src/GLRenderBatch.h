#pragma once

#include "GLDrawCall.h"
#include "GLVertexBuffer.h"

#include <vector>

#include <glm/glm.hpp>

struct GLRenderBatch {
public:
	GLRenderBatch() {}
	~GLRenderBatch() {}

	void DrawLine(glm::vec3 x1, glm::vec3 x2, glm::vec4 color);
	void DrawTriangle(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color);
	// TODO: 这个函数后期要进行修改再包装
	void DrawRectangleTemp(glm::vec3 bottomLeft, float width, float height, glm::vec4 color);

public:
	void Color4f(float r, float g, float b, float a);
	void Vertex3f(float x, float y, float z);
	void BeginDrawMode(int mode);
	void EndDrawMode();
	void LoadRenderBatch(int bufferCount, int elementCount);
	void DrawRenderBatch();
	bool CheckRenderBatchLimit(int vCount);
	// TODO: 
	// void UnloadRenderBatch(); 

public:
	unsigned int m_CurrentBufferIdx = 0;
	unsigned int m_CurrentShaderId = 0;
	// TODO: glm 
	glm::vec4 m_CurrentColor{ 1.0f, 1.0f,1.0f, 1.0f };
	int m_VertexCount = 0;
	float m_CurrentDepth = -1.0f;
	float m_DepthDelta = 1.0f / 20000.0f;

	int m_BufferCount = 5;
	int m_ElementCount = 1024;
	int m_DrawCallCount = 256;
	int m_DrawCounter = 1;

	std::vector<GLVertexBuffer> m_Buffers;
	std::vector<GLDrawCall> m_DrawCalls;
};