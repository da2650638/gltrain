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

void GLRenderBatch::DrawTriangleLines(glm::vec2 x1, glm::vec2 x2, glm::vec2 x3, glm::vec4 color)
{
	DrawTriangleLines({ x1.x, x1.y, m_CurrentDepth }, { x2.x, x2.y, m_CurrentDepth }, { x3.x, x3.y, m_CurrentDepth }, color);
}

void GLRenderBatch::DrawTriangleLines(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color)
{
	BeginDrawMode(LINES);
	{
		Color4f(color.r, color.g, color.b, color.a);

		Vertex3f(x1.x, x1.y, x1.z);
		Vertex3f(x2.x, x2.y, x2.z);

		Vertex3f(x2.x, x2.y, x2.z);
		Vertex3f(x3.x, x3.y, x3.z);

		Vertex3f(x3.x, x3.y, x3.z);
		Vertex3f(x1.x, x1.y, x1.z);
	}
	EndDrawMode();
}

void GLRenderBatch::DrawTriangle(glm::vec2 x1, glm::vec2 x2, glm::vec2 x3, glm::vec4 color)
{
	DrawTriangle({ x1.x, x1.y, m_CurrentDepth }, { x2.x, x2.y, m_CurrentDepth }, { x3.x, x3.y, m_CurrentDepth }, color);
}

void GLRenderBatch::DrawTriangle(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec4 color)
{
	//BeginDrawMode(TRIANGLES);
	//Color4f(color.r, color.g, color.b, color.a);
	//Vertex3f(x1.x, x1.y, x1.z);
	//Vertex3f(x2.x, x2.y, x2.z);
	//Vertex3f(x3.x, x3.y, x3.z);
	//EndDrawMode();
	BeginDrawMode(QUADS);
		Color4f(color.r, color.g, color.b, color.a);
		Vertex2f(x1.x, x1.y);
		Vertex2f(x2.x, x2.y);
		Vertex2f(x2.x, x2.y);
		Vertex2f(x3.x, x3.y);
	EndDrawMode();
}

void GLRenderBatch::DrawRectangleLines(int posX, int posY, int width, int height, glm::vec4 color)
{
	// TODO: 不需要考虑旋转吗？
	BeginDrawMode(LINES);
	{
		Color4f(color.r, color.g, color.b, color.a);
		Vertex2f((float)posX, (float)posY);
		Vertex2f((float)posX + (float)width, (float)posY + 1);

		Vertex2f((float)posX + (float)width, (float)posY + 1);
		Vertex2f((float)posX + (float)width, (float)posY + (float)height);

		Vertex2f((float)posX + (float)width, (float)posY + (float)height);
		Vertex2f((float)posX + 1, (float)posY + (float)height);

		Vertex2f((float)posX + 1, (float)posY + (float)height);
		Vertex2f((float)posX + 1, (float)posY + 1);
	}
	EndDrawMode();
}

void GLRenderBatch::DrawRectangle(int posX, int posY, int width, int height, glm::vec4 color)
{
	DrawRectangleV({ (float)posX, (float)posY }, { (float)width, (float)height }, color);
}

void GLRenderBatch::DrawRectangleV(GLMath::Vector2 position, GLMath::Vector2 size, glm::vec4 color)
{
	DrawRectanglePro({ position.x, position.y, size.x, size.y }, { 0.0f, 0.0f }, 0.0f, color);
}

void GLRenderBatch::DrawRectanglePro(GLMath::Rectangle rec, GLMath::Vector2 origin, float rotation, glm::vec4 color)
{
	GLMath::Vector2 topLeft = { 0.0f, 0.0f };
	GLMath::Vector2 topRight = { 0.0f, 0.0f };
	GLMath::Vector2 bottomRight = { 0.0f, 0.0f };
	GLMath::Vector2 bottomLeft = { 0.0f, 0.0f };
	if (rotation == 0.0f)
	{
		float left = rec.x - origin.x;
		float top = rec.y - origin.y;
		topLeft = { left, top };
		topRight = { left + rec.width, top };
		bottomRight = { left + rec.width, top + rec.height };
		bottomLeft = { left, top + rec.height };
	}
	else
	{
		// TODO: 感官上glm应该是拖慢了速度，需要优化
		float cosRotation = std::cos(glm::radians(rotation));
		float sinRotation = std::sin(glm::radians(rotation));
		float dx = -origin.x;
		float dy = -origin.y;
		float x = rec.x;
		float y = rec.y;

		topLeft.x = x + dx * cosRotation - dy * sinRotation;
		topLeft.y = y + dx * sinRotation + dy * cosRotation;

		topRight.x = x + (dx + rec.width) * cosRotation - dy * sinRotation;
		topRight.y = y + (dx + rec.width) * sinRotation + dy * cosRotation;

		bottomRight.x = x + (dx + rec.width) * cosRotation - (dy + rec.height) * sinRotation;
		bottomRight.y = y + (dx + rec.width) * sinRotation + (dy + rec.height) * cosRotation;

		bottomLeft.x = x + dx * cosRotation - (dy + rec.height) * sinRotation;
		bottomLeft.y = y + dx * sinRotation + (dy + rec.height) * cosRotation;
	}

	BeginDrawMode(QUADS);
	Color4f(color.r, color.g, color.b, color.a);
	Vertex2f(topLeft.x, topLeft.y);
	Vertex2f(topRight.x, topRight.y);
	Vertex2f(bottomRight.x, bottomRight.y);
	Vertex2f(bottomLeft.x, bottomLeft.y);
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
