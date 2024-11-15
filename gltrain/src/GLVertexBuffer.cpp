#include "GLVertexBuffer.h"

void GLVertexBuffer::Init(int elementCount)
{
	if (m_Vao == 0)
	{
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		auto shaderInfo = GLGlobalState::GetInstance().LoadShaderInfos("resource/config/shader.yml", "default shader");

		m_Positions.resize(elementCount * 4 * 3);
		glCreateBuffers(1, &m_PosVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosVbo);
		glBufferData(GL_ARRAY_BUFFER, elementCount * 4 * 3 * sizeof(float), m_Positions.data() , GL_DYNAMIC_DRAW);
		GLGlobalState::LocationData posData = shaderInfo.LocationDatas["vertexPosition"];
		glEnableVertexAttribArray(posData.DefaultLoc);
		glVertexAttribPointer(posData.DefaultLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		m_Colors.resize(elementCount * 4 * 4);
		glCreateBuffers(1, &m_ColVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_ColVbo);
		glBufferData(GL_ARRAY_BUFFER, elementCount * 4 * 4 * sizeof(float), m_Colors.data(), GL_DYNAMIC_DRAW);
		GLGlobalState::LocationData colData = shaderInfo.LocationDatas["vertexColor"];
		glEnableVertexAttribArray(colData.DefaultLoc);
		glVertexAttribPointer(colData.DefaultLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

		m_Indices.resize(elementCount * 6);
		for (int i = 0; i < elementCount; i ++)
		{
			m_Indices[i * 6 + 0] = i * 4 + 0;
			m_Indices[i * 6 + 1] = i * 4 + 1;
			m_Indices[i * 6 + 2] = i * 4 + 2;
			m_Indices[i * 6 + 3] = i * 4 + 0;
			m_Indices[i * 6 + 4] = i * 4 + 2;
			m_Indices[i * 6 + 5] = i * 4 + 3;
		}
		glCreateBuffers(1, &m_IdxVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IdxVbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * 6 * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

void GLVertexBuffer::Shutdown()
{
	// TODO: 清理资源应该如何去做呢？？？
}