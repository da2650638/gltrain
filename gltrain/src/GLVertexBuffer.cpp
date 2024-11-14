#include "GLVertexBuffer.h"

void GLVertexBuffer::Init(int elementCount)
{
	if (m_Vao == 0)
	{
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		m_Positions.resize(elementCount * 4 * 3);
		glCreateBuffers(1, &m_PosVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_PosVbo);
		glBufferData(GL_ARRAY_BUFFER, elementCount * 4 * 3 * sizeof(float), m_Positions.data() , GL_DYNAMIC_DRAW);
		// TODO: 这里attrib的index应该进行灵活的配置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		m_Colors.resize(elementCount * 4 * 4);
		glCreateBuffers(1, &m_ColVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_ColVbo);
		glBufferData(GL_ARRAY_BUFFER, elementCount * 4 * 4 * sizeof(float), m_Colors.data(), GL_DYNAMIC_DRAW);
		// TODO: 这里attrib的index应该进行灵活的配置
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}
}

void GLVertexBuffer::Shutdown()
{
	// TODO: 清理资源应该如何去做呢？？？
}