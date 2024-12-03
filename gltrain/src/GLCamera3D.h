#pragma once
#include "GLGlobal.h"
#include "GLMath.h"
#include "glm/glm.hpp"

// TODO: 以后需要使用自己的数学库
class GLCamera3D {
public:
	GLCamera3D();
	~GLCamera3D();

	void SetPosition(glm::vec3 position)
	{
		m_Position = position;
	}
	void SetTarget(glm::vec3 target)
	{
		m_Target = target;
	}
	void SetUp(glm::vec3 up)
	{
		m_Up = up;
	}
	void SetFovy(float fovy)
	{
		m_Fovy = fovy;
	}
	void SetProjection(int projectionType)
	{
		m_Projection = projectionType;
	}

	void UpdateCamera();

private:
	glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
	glm::vec3 m_Target{0.0f, 0.0f, 0.0f};
	glm::vec3 m_Up{0.0f, 0.0f, 0.0f};
	float m_Fovy = 0.0f;
	int m_Projection = 0;
};

using GLCamera = GLCamera3D;
