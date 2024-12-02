#pragma once
#include "GLGlobal.h"
#include "GLMath.h"
#include "glm/glm.hpp"

// TODO: �Ժ���Ҫʹ���Լ�����ѧ��
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
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Up;
	float m_Fovy;
	int m_Projection;
};

using GLCamera = GLCamera3D;
