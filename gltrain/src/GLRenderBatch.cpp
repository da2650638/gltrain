#include "GLRenderBatch.h"

#include <algorithm>

void GLRenderBatch::DrawLine(glm::vec3 x1, glm::vec3 x2, glm::vec4 color)
{
	BeginDrawMode(LINES);
	Color4f(color.r, color.g, color.b, color.a);
	Vertex3f(x1.x, x1.y, x1.z);
	Vertex3f(x2.x, x2.y, x2.z);
	EndDrawMode();
}

void GLRenderBatch::DrawTriangle(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color)
{
	BeginDrawMode(TRIANGLES);
	Color4f(color.r, color.g, color.b, color.a);
	Vertex3f(x1.x, x1.y, x1.z);
	Vertex3f(x2.x, x2.y, x2.z);
	Vertex3f(x3.x, x3.y, x3.z);
	EndDrawMode();
}

void GLRenderBatch::DrawRectangleTemp(glm::vec3 bottomLeft, float width, float height, glm::vec4 color)
{
	BeginDrawMode(QUADS);
	glm::vec3 bottomRight(bottomLeft.x + width, bottomLeft.y, bottomLeft.z);
	glm::vec3 topRight(bottomLeft.x + width, bottomLeft.y + height, bottomLeft.z);
	glm::vec3 topLeft(bottomLeft.x, bottomLeft.y + height, bottomLeft.z);
	Color4f(color.r, color.g, color.b, color.a);
	Vertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
	Vertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
	Vertex3f(topRight.x, topRight.y, topRight.z);
	Vertex3f(topLeft.x, topLeft.y, topLeft.z);
	EndDrawMode();
}

void GLRenderBatch::Color4f(float r, float g, float b, float a)
{
	m_CurrentColor.r = std::clamp(r, 0.0f, 1.0f);
	m_CurrentColor.g = std::clamp(g, 0.0f, 1.0f);
	m_CurrentColor.b = std::clamp(b, 0.0f, 1.0f);
	m_CurrentColor.a = std::clamp(a, 0.0f, 1.0f);
}

void GLRenderBatch::Vertex3f(float x, float y, float z)
{
	float tx = x;
	float ty = y;
	float tz = z;

	// TODO: transform

	if (m_VertexCount > m_ElementCount * 4 - 4)
	{
		if (m_DrawCalls[m_DrawCounter - 1].Mode == LINES && m_DrawCalls[m_DrawCounter - 1].VertexCount % 2 == 0)
		{
			CheckRenderBatchLimit(2 + 1);
		}
		else if (m_DrawCalls[m_DrawCounter - 1].Mode == TRIANGLES && m_DrawCalls[m_DrawCounter - 1].VertexCount % 3 == 0)
		{
			CheckRenderBatchLimit(3 + 1);
		}
		else if (m_DrawCalls[m_DrawCounter - 1].Mode == QUADS && m_DrawCalls[m_DrawCounter - 1].VertexCount % 4 == 0)
		{
			CheckRenderBatchLimit(4 + 1);
		}
	}

	m_Buffers[m_CurrentBufferIdx].m_Positions[3 * m_VertexCount + 0] = tx;
	m_Buffers[m_CurrentBufferIdx].m_Positions[3 * m_VertexCount + 1] = ty;
	m_Buffers[m_CurrentBufferIdx].m_Positions[3 * m_VertexCount + 2] = tz;

	m_Buffers[m_CurrentBufferIdx].m_Colors[4 * m_VertexCount + 0] = m_CurrentColor.r;
	m_Buffers[m_CurrentBufferIdx].m_Colors[4 * m_VertexCount + 1] = m_CurrentColor.g;
	m_Buffers[m_CurrentBufferIdx].m_Colors[4 * m_VertexCount + 2] = m_CurrentColor.b;
	m_Buffers[m_CurrentBufferIdx].m_Colors[4 * m_VertexCount + 3] = m_CurrentColor.a;

	m_VertexCount++;
	m_DrawCalls[m_DrawCounter - 1].VertexCount++;
}

void GLRenderBatch::Vertex2f(float x, float y)
{
	Vertex3f(x, y, m_CurrentDepth);
}

void GLRenderBatch::BeginDrawMode(int mode)
{
	if (mode != m_DrawCalls[m_DrawCounter - 1].Mode)
	{
		if (m_DrawCalls[m_DrawCounter - 1].VertexCount > 0)
		{
			if (m_DrawCalls[m_DrawCounter - 1].Mode == LINES)
			{
				if (m_DrawCalls[m_DrawCounter - 1].VertexCount % 4 == 0)
					m_DrawCalls[m_DrawCounter - 1].VertexAlignment = 0;
				else
					m_DrawCalls[m_DrawCounter - 1].VertexAlignment = 2;
			}
			else if (m_DrawCalls[m_DrawCounter - 1].Mode == TRIANGLES)
			{
				if (m_DrawCalls[m_DrawCounter - 1].VertexCount % 4 == 0)
					m_DrawCalls[m_DrawCounter - 1].VertexAlignment = 0;
				else
					m_DrawCalls[m_DrawCounter - 1].VertexAlignment = (4 - m_DrawCalls[m_DrawCounter - 1].VertexCount % 4);
			}
			else if (m_DrawCalls[m_DrawCounter - 1].Mode == QUADS)
			{
				m_DrawCalls[m_DrawCounter - 1].VertexAlignment = 0;
			}

			if (!CheckRenderBatchLimit(m_DrawCalls[m_DrawCounter - 1].VertexAlignment))
			{
				m_VertexCount += m_DrawCalls[m_DrawCounter - 1].VertexAlignment;
				m_DrawCounter++;
			}
		}

		m_DrawCalls[m_DrawCounter - 1].Mode = mode;
		m_DrawCalls[m_DrawCounter - 1].TextureID = 0;
		m_DrawCalls[m_DrawCounter - 1].VertexCount = 0;
		m_DrawCalls[m_DrawCounter - 1].VertexAlignment = 0;
	}
}

void GLRenderBatch::EndDrawMode()
{
	m_CurrentDepth += m_DepthDelta;
}

void GLRenderBatch::LoadRenderBatch(int bufferCount, int elementCount)
{
	m_BufferCount = bufferCount;
	m_ElementCount = elementCount;

	// TODO: resize 是否包含了创建对象的过程？？
	m_Buffers.resize(m_BufferCount);
	for (int i = 0; i < m_BufferCount; i++)
	{
		m_Buffers[i].Init(m_ElementCount);
	}

	// TODO: m_DrawCallCount也应该动态的去控制
	m_DrawCalls.resize(m_DrawCallCount);
	for (int i = 0; i < m_DrawCallCount; i++)
	{
		m_DrawCalls[i].Mode = QUADS;
		m_DrawCalls[i].TextureID = 0;
		m_DrawCalls[i].VertexAlignment = 0;
		m_DrawCalls[i].VertexCount = 0;
	}
}

void GLRenderBatch::DrawRenderBatch()
{
	if (m_VertexCount > 0)
	{
		glBindVertexArray(m_Buffers[m_CurrentBufferIdx].m_Vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[m_CurrentBufferIdx].m_PosVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * 3 * sizeof(float), m_Buffers[m_CurrentBufferIdx].m_Positions.data());

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[m_CurrentBufferIdx].m_ColVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * 4 * sizeof(float), m_Buffers[m_CurrentBufferIdx].m_Colors.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[m_CurrentBufferIdx].m_IdxVbo);

		glBindVertexArray(0);
	}

	if (m_VertexCount > 0)
	{
		glBindVertexArray(m_Buffers[m_CurrentBufferIdx].m_Vao);
		glUseProgram(m_CurrentShaderId);

		for (int i = 0, vertexOffset = 0; i < m_DrawCounter; i++)
		{
			if (m_DrawCalls[i].Mode == LINES || m_DrawCalls[i].Mode == TRIANGLES)
			{
				glDrawArrays(m_DrawCalls[i].Mode, vertexOffset, m_DrawCalls[i].VertexCount);
			}
			else if (m_DrawCalls[i].Mode == QUADS)
			{
				// TODO: 这里不应该使用GL_QUADS而是使用GL_TRIANGLES
				glDrawElements(GL_TRIANGLES, m_DrawCalls[i].VertexCount / 4 * 6, GL_UNSIGNED_INT, (GLvoid*)(vertexOffset / 4 * 6 * sizeof(unsigned int)));
			}
			vertexOffset += (m_DrawCalls[i].VertexCount + m_DrawCalls[i].VertexAlignment);
		}
		
		glUseProgram(0);
		glBindVertexArray(0);
	}

	m_DrawCounter = 1;
	m_VertexCount = 0;
	m_CurrentDepth = -1.0f;
	for (int i = 0; i < m_DrawCallCount; i++)
	{
		m_DrawCalls[i].VertexCount = 0;
		m_DrawCalls[i].VertexAlignment = 0;
	}
}

bool GLRenderBatch::CheckRenderBatchLimit(int vCount)
{
	bool overflow = false;

	if (m_VertexCount + vCount >= m_ElementCount * 4)
	{
		overflow = true;

		int currentMode = m_DrawCalls[m_DrawCounter - 1].Mode;
		int currentTexture = m_DrawCalls[m_DrawCounter - 1].TextureID;

		DrawRenderBatch();

		m_DrawCalls[m_DrawCounter - 1].Mode = currentMode;
		m_DrawCalls[m_DrawCounter - 1].TextureID = currentTexture;
	}

	return overflow;
}
