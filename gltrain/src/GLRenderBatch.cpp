#include "GLRenderBatch.h"

#include <algorithm>
#include <numbers>

void GLRenderBatch::DrawLine(glm::vec3 x1, glm::vec3 x2, glm::vec4 color)
{
	// TODO: 后期加上纹理
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
	// TODO: 后期加上纹理
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
	// TODO: 后期加上纹理
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
	// TODO: 后期加上纹理
	// TODO: 不需要考虑旋转吗？
	BeginDrawMode(LINES);
	{
		Color4f(color.r, color.g, color.b, color.a);

		Vertex2f((float)posX, (float)posY);
		Vertex2f((float)(posX + width), (float)(posY + 1));

		Vertex2f((float)(posX + width), (float)(posY + 1));
		Vertex2f((float)(posX + width), (float)(posY + height));

		Vertex2f((float)(posX + width), (float)(posY + height));
		Vertex2f((float)(posX + 1), (float)(posY + height));

		Vertex2f((float)(posX + 1), (float)(posY + height));
		Vertex2f((float)(posX + 1), (float)(posY + +1));
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
	// TODO: 后期加上纹理
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

void GLRenderBatch::DrawRectangleGradientV(int posX, int posY, int width, int height, glm::vec4 topc, glm::vec4 bottomc)
{
	DrawRectangleGradientEx({ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(width), static_cast<float>(height) }, topc, bottomc, topc, bottomc);
}

void GLRenderBatch::DrawRectangleGradientH(int posX, int posY, int width, int height, glm::vec4 leftc, glm::vec4 rightc)
{
	DrawRectangleGradientEx({ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(width), static_cast<float>(height) }, leftc, leftc, rightc, rightc);
}

void GLRenderBatch::DrawRectangleGradientEx(GLMath::Rectangle rec, glm::vec4 topLeftc, glm::vec4 bottomLeftc, glm::vec4 topRightc, glm::vec4 bottomRightc, GLMath::Vector2 origin, float rotation)
{
	// TODO: 后期加上纹理
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

	Color4f(topLeftc.r, topLeftc.g, topLeftc.b, topLeftc.a);
	Vertex2f(topLeft.x, topLeft.y);

	Color4f(topRightc.r, topRightc.g, topRightc.b, topRightc.a);
	Vertex2f(topRight.x, topRight.y);

	Color4f(bottomRightc.r, bottomRightc.g, bottomRightc.b, bottomRightc.a);
	Vertex2f(bottomRight.x, bottomRight.y);

	Color4f(bottomLeftc.r, bottomLeftc.g, bottomLeftc.b, bottomLeftc.a);
	Vertex2f(bottomLeft.x, bottomLeft.y);

	EndDrawMode();
}

void GLRenderBatch::DrawPoly(glm::vec2 origin, int sides, float radius, float rotation, glm::vec4 color)
{
	// TODO: 后续考虑如何添加纹理
	if (sides < 3) sides = 3;
	float rotationStep = glm::radians(360.f / sides);
	float initRotation = rotationStep / 2.0f + glm::radians(rotation);
	float currentRotation = initRotation;
	BeginDrawMode(QUADS);
	Color4f(color.r, color.g, color.b, color.a);
	for (int i = 0; i < sides; i++)
	{
		float sinTheta = std::sin(currentRotation);
		float cosTheta = std::cos(currentRotation);
		float sinAlpha = std::sin(currentRotation + rotationStep);
		float cosAlpha = std::cos(currentRotation + rotationStep);
		Vertex2f(origin.x, origin.y);

		Vertex2f(origin.x - sinTheta * radius, origin.y + cosTheta * radius);

		Vertex2f(origin.x - sinTheta * radius, origin.y + cosTheta * radius);

		Vertex2f(origin.x - sinAlpha * radius, origin.y + cosAlpha * radius);

		currentRotation += rotationStep;
	}
	EndDrawMode();

	//BeginDrawMode(TRIANGLES);
	//for (int i = 0; i < sides; i++)
	//{
	//	float sinTheta = std::sin(currentRotation);
	//	float cosTheta = std::cos(currentRotation);
	//	float sinAlpha = std::sin(currentRotation + rotationStep);
	//	float cosAlpha = std::cos(currentRotation + rotationStep);
	//	Color4f(color.r, color.g, color.b, color.a);
	//	Vertex2f(origin.x, origin.y);

	//	Vertex2f(origin.x - sinTheta * radius, origin.y + cosTheta * radius);

	//	Vertex2f(origin.x - sinAlpha * radius, origin.y + cosAlpha * radius);

	//	currentRotation += rotationStep;
	//}
	//EndDrawMode();
}

void GLRenderBatch::DrawPolyLines(glm::vec2 origin, int sides, float radius, float rotation, glm::vec4 color)
{
	// TODO: 后续考虑如何添加纹理
	if (sides < 3) sides = 3;
	float rotationStep = glm::radians(360.f / sides);
	float initRotation = rotationStep / 2.0f + glm::radians(rotation);
	float currentRotation = initRotation;
	BeginDrawMode(LINES);
	Color4f(color.r, color.g, color.b, color.a);
	for (int i = 0; i < sides; i++)
	{
		float sinTheta = std::sin(currentRotation);
		float cosTheta = std::cos(currentRotation);
		float sinAlpha = std::sin(currentRotation + rotationStep);
		float cosAlpha = std::cos(currentRotation + rotationStep);

		Vertex2f(origin.x - sinTheta * radius, origin.y + cosTheta * radius);

		Vertex2f(origin.x - sinAlpha * radius, origin.y + cosAlpha * radius);

		currentRotation += rotationStep;
	}
	EndDrawMode();
}

void GLRenderBatch::DrawPolyLinesEx(glm::vec2 origin, int sides, float radius, float rotation, float lineThick, glm::vec4 color)
{
	// TODO: 后续考虑如何添加纹理
	if (sides < 3) sides = 3;
	float rotationStep = glm::radians(360.f / sides);
	float initRotation = rotationStep / 2.0f + glm::radians(rotation);
	float currentRotation = initRotation;
	float innerRadius = radius - lineThick * std::cos( glm::radians(rotationStep / 2.0f) );
	BeginDrawMode(QUADS);
	Color4f(color.r, color.g, color.b, color.a);
	for (int i = 0; i < sides; i++)
	{
		float sinTheta = std::sin(currentRotation);
		float cosTheta = std::cos(currentRotation);
		float sinAlpha = std::sin(currentRotation + rotationStep);
		float cosAlpha = std::cos(currentRotation + rotationStep);

		Vertex2f(origin.x - sinTheta * radius, origin.y + cosTheta * radius);

		Vertex2f(origin.x - sinTheta * innerRadius, origin.y + cosTheta * innerRadius);

		Vertex2f(origin.x - sinAlpha * innerRadius, origin.y + cosAlpha * innerRadius);

		Vertex2f(origin.x - sinAlpha * radius, origin.y + cosAlpha * radius);

		currentRotation += rotationStep;
	}
	EndDrawMode();
}

void GLRenderBatch::DrawCircle(int centerX, int centerY, float radius, glm::vec4 color)
{
	DrawCircleV({ (float)centerX, (float)centerY }, radius, color);
}

void GLRenderBatch::DrawCircleV(GLMath::Vector2 center, float radius, glm::vec4 color)
{
	DrawCircleSectors(center, radius, 0.0, 360.f, 36, color);
}

void GLRenderBatch::DrawCircleSectors(GLMath::Vector2 center, float radius, float startAngle, float endAngle, int segments, glm::vec4 color)
{
	// TODO: 后续考虑一下纹理
	if (radius <= 0.0f) radius = 0.1;

	if (startAngle > endAngle)
	{
		std::swap(startAngle, endAngle);
	}

	// 了解一下std::ceilf的计算原理以及std::floor
	int minSegments = std::ceilf((endAngle - startAngle) / 90.0f);

	if (segments < minSegments)
	{
		// TODO:这个0.5f需要改成可配置的参数
		float theta = std::acos(2 * std::pow(1 - 0.5f / radius, 2.0f) - 1.0f);
		segments = static_cast<int>(std::ceilf(2 * std::numbers::pi / theta) * (endAngle - startAngle) / 360.f);
		if (segments <= 0) segments = minSegments;
	}

	float stepAngle = (endAngle - startAngle) / segments;
	float angle = startAngle;
	BeginDrawMode(QUADS);
	Color4f(color.r, color.g, color.b, color.a);
	for (int i = 0; i < (segments / 2); i++)
	{
		float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
		float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));
		float cosGamma = std::cos(glm::radians(angle + 2 * stepAngle)), sinGamma = std::sin(glm::radians(angle + 2 * stepAngle));

		Vertex2f(center.x, center.y);

		Vertex2f(center.x + radius * cosAlpha, center.y + radius * sinAlpha);

		Vertex2f(center.x + radius * cosTheta, center.y + radius * sinTheta);

		Vertex2f(center.x + radius * cosGamma, center.y + radius * sinGamma);

		angle += 2 * stepAngle;
	}

	if ((segments % 2) == 1)
	{
		float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
		float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));

		Vertex2f(center.x, center.y);

		Vertex2f(center.x + radius * cosAlpha, center.y + radius * sinAlpha);

		Vertex2f(center.x + radius * cosTheta, center.y + radius * sinTheta);

		Vertex2f(center.x, center.y);

		angle += stepAngle;
	}
	EndDrawMode();

	//BeginDrawMode(TRIANGLES);
	//for (int i = 0; i < segments; i++)
	//{
	//	float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
	//	float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));
	//	Color4f(color.r, color.g, color.b, color.a);

	//	Vertex2f(center.x, center.y);

	//	Vertex2f(center.x + radius * cosAlpha, center.y + radius * sinAlpha);

	//	Vertex2f(center.x + radius * cosTheta, center.y + radius * sinTheta);

	//	angle += stepAngle;
	//}
	//EndDrawMode();
}

void GLRenderBatch::DrawCircleGradient(int centerX, int centerY, float radius, glm::vec4 inner, glm::vec4 outer)
{
	DrawCircleGradientV({ (float)centerX, (float)centerY }, radius, inner, outer);
}

void GLRenderBatch::DrawCircleGradientV(GLMath::Vector2 center, float radius, glm::vec4 inner, glm::vec4 outer)
{
	DrawCircleSectorsGradient(center, radius, inner, outer);
}

void GLRenderBatch::DrawCircleSectorsGradient(GLMath::Vector2 center, float radius, glm::vec4 inner, glm::vec4 outer, float startAngle, float endAngle, int segments)
{
	// TODO: 后续考虑一下纹理
	if (radius <= 0.0f) radius = 0.1;

	if (startAngle > endAngle)
	{
		std::swap(startAngle, endAngle);
	}

	// 了解一下std::ceilf的计算原理以及std::floor
	int minSegments = std::ceilf((endAngle - startAngle) / 90.0f);

	if (segments < minSegments)
	{
		// TODO:这个0.5f需要改成可配置的参数
		float theta = std::acos(2 * std::pow(1 - 0.5f / radius, 2.0f) - 1.0f);
		segments = static_cast<int>(std::ceilf(2 * std::numbers::pi / theta) * (endAngle - startAngle) / 360.f);
		if (segments <= 0) segments = minSegments;
	}

	float stepAngle = (endAngle - startAngle) / segments;
	float angle = startAngle;
	BeginDrawMode(QUADS);
	for (int i = 0; i < (segments / 2); i++)
	{
		float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
		float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));
		float cosGamma = std::cos(glm::radians(angle + 2 * stepAngle)), sinGamma = std::sin(glm::radians(angle + 2 * stepAngle));

		Color4f(inner.r, inner.g, inner.b, inner.a);
		Vertex2f(center.x, center.y);

		Color4f(outer.r, outer.g, outer.b, outer.a);
		Vertex2f(center.x + radius * cosAlpha, center.y + radius * sinAlpha);

		Vertex2f(center.x + radius * cosTheta, center.y + radius * sinTheta);

		Vertex2f(center.x + radius * cosGamma, center.y + radius * sinGamma);

		angle += 2 * stepAngle;
	}

	if ((segments % 2) == 1)
	{
		float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
		float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));

		Vertex2f(center.x, center.y);

		Vertex2f(center.x + radius * cosAlpha, center.y + radius * sinAlpha);

		Vertex2f(center.x + radius * cosTheta, center.y + radius * sinTheta);

		Vertex2f(center.x, center.y);

		angle += stepAngle;
	}
	EndDrawMode();
}

void GLRenderBatch::DrawCircleLines(int centerX, int centerY, float radius, glm::vec4 color)
{
	DrawCircleLinesV({ static_cast<float>(centerX), static_cast<float>(centerY) }, radius, color);
}

void GLRenderBatch::DrawCircleLinesV(GLMath::Vector2 center, float radius, glm::vec4 color)
{
	DrawCircleLineSectors(center, radius, color);
}

void GLRenderBatch::DrawCircleLineSectors(GLMath::Vector2 center, float radius, glm::vec4 color, float startAngle, float endAngle, int segments)
{
	// TODO: 后续考虑一下纹理
	if (radius < 0.0f) radius = 0.1f;

	if (endAngle < startAngle) std::swap(startAngle, endAngle);

	int minSegments = std::ceilf((endAngle - startAngle) / 90.0f);

	if (segments < minSegments)
	{
		// TODO: 这个0.5需要改成可配置的参数
		float theta = std::acos(2 * std::pow(1.0f - 0.5f / radius, 2) - 1.0f);
		segments = (int)(std::ceilf(2 * std::numbers::pi / theta) * (endAngle - startAngle) / 360.0f);
		if (segments <= 0) segments = minSegments;
	}

	float angle = startAngle;
	float stepAngle = (endAngle - startAngle) / segments;
	BeginDrawMode(LINES);
	Color4f(color.r, color.g, color.b, color.a);
	for (int i = 0; i < segments; i++)
	{
		float cosAlpha = std::cos(glm::radians(angle)), sinAlpha = std::sin(glm::radians(angle));
		float cosTheta = std::cos(glm::radians(angle + stepAngle)), sinTheta = std::sin(glm::radians(angle + stepAngle));
		Vertex2f(center.x + cosAlpha * radius, center.y + sinAlpha * radius);
		Vertex2f(center.x + cosTheta * radius, center.y + sinTheta * radius);
		angle += stepAngle;
	}
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
