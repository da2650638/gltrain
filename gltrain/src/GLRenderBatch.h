#pragma once

#include "GLDrawCall.h"
#include "GLVertexBuffer.h"
#include "GLMath.h"

#include <vector>

#include <glm/glm.hpp>

struct GLRenderBatch {
public:
	GLRenderBatch() {}
	~GLRenderBatch() {}

	// TODO: 这部分绘制函数是否需要单独抽离出来呢？？
	// TODO: glm后期要换成自己写的数学相关数据结构
	void DrawLine(glm::vec3 x1, glm::vec3 x2, glm::vec4 color);

	// TODO: glm后期要换成自己写的数学相关数据结构
	void DrawTriangleLines(glm::vec2 x1, glm::vec2 x2, glm::vec2 x3, glm::vec4 color);
	void DrawTriangleLines(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color);
	void DrawTriangle(glm::vec2 x1, glm::vec2 x2, glm::vec2 x3, glm::vec4 color);
	void DrawTriangle(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color);

	// TODO: glm后期要换成自己写的数学相关数据结构
	void DrawRectangleLines(int posX, int posY, int width, int height, glm::vec4 color);
	void DrawRectangle(int posX, int posY, int width, int height, glm::vec4 color);
	void DrawRectangleV(GLMath::Vector2 position, GLMath::Vector2 size, glm::vec4 color);
	void DrawRectanglePro(GLMath::Rectangle rec, GLMath::Vector2 origin, float rotation, glm::vec4 color);

	// TODO: glm后期要换成自己写的数学相关数据结构
	void DrawPoly(glm::vec2 origin, int sides, float radius, float rotation, glm::vec4 color);
	void DrawPolyLines(glm::vec2 origin, int sides, float radius, float rotation, glm::vec4 color);
	void DrawPolyLinesEx(glm::vec2 origin, int sides, float radius, float rotation, float lineThick, glm::vec4 color);

public:
	void Color4f(float r, float g, float b, float a);
	void Vertex3f(float x, float y, float z);
	void Vertex2f(float x, float y);
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