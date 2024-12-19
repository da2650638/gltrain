#include "GLRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>

#include "Casic/CasicMatrixTransform.h"

namespace Casic
{
namespace GL
{
	std::once_flag GLRenderer::m_InitFlag;
	std::unique_ptr<GLRenderer> GLRenderer::m_Instance;

	GLRenderer& GLRenderer::GetInstance()
	{
		std::call_once(m_InitFlag, []() {
			m_Instance.reset(new GLRenderer());
		});
		return *m_Instance;
	}

	GLRenderer::~GLRenderer()
	{
	}

	void GLRenderer::SetPlatform(GLPlatform* platform)
	{
		m_PlatformInst = platform;
	}

	void GLRenderer::SetRenderData(RenderData rd)
	{
		m_RenderData = rd;
	}

	void GLRenderer::SetRenderSize(int width, int height)
	{
		m_RenderData.Width = width;
		m_RenderData.Height = height;
		SetupViewport(width, height);
	}

	void GLRenderer::RenderInit()
	{
		//---------------------------------------------------------------
		// 初始化Texture，初始化RenderBatch，初始化Shader，初始化内部矩阵，初始化OpenGL某些选项
		//---------------------------------------------------------------

		//---------------------------------------------------------------
		// TODO: 初始化Texture
		//---------------------------------------------------------------

		//---------------------------------------------------------------
		// TODO: 初始化shader
		//---------------------------------------------------------------
		m_DefaultShader.LoadShader("res/shader/defaultVS.glsl", "res/shader/defaultFS.glsl");
		SimpleLogger::GetInstance().Info("Get vertex position loc:{}", m_DefaultShader.GetAttribLocation("vertexPosition"));
		SimpleLogger::GetInstance().Info("Get vertex tex coord loc:{}", m_DefaultShader.GetAttribLocation("vertexTexcoord"));
		SimpleLogger::GetInstance().Info("Get vertex normal loc:{}", m_DefaultShader.GetAttribLocation("vertexNormal"));
		SimpleLogger::GetInstance().Info("Get vertex color loc:{}", m_DefaultShader.GetAttribLocation("vertexColor"));
		SimpleLogger::GetInstance().Info("Get mvp location: {}", m_DefaultShader.GetUniformLocation("mvp"));
		m_CurrentShader = &m_DefaultShader;

		//---------------------------------------------------------------
		// TODO: 初始化RenderBatch
		//---------------------------------------------------------------
		LoadRenderBatch();

		//----------------------------------------------------------------
		// 初始化内部矩阵
		//----------------------------------------------------------------
		m_Projection = m_ModelView = m_Transform = Math::Matrix4::Identity();
		m_CurrentMatrix = &m_ModelView;

		//---------------------------------------------------------------
		// TODO: 初始化OpenGL某些选项
		//---------------------------------------------------------------
	}

	// NOTE: 这个功能实在Platform模块还是Renderer模块呢？我也不太清楚
	void GLRenderer::SetTargetFps(unsigned int fps)
	{
		if (fps == 0) return;
		m_RenderData.TargetFps = fps;
		BASIC_RUNTIME_CHECK(m_PlatformInst != nullptr, "GLRenderer instance need a platform instance");
		float target = 1.0f / (float)fps;
		m_PlatformInst->TimeData().Target = target;
	}

	void GLRenderer::SetupViewport(int width, int height)
	{
		m_RenderData.Width = width;
		m_RenderData.Height = height;
		UpdateViewport();

		MatrixMode(PROJECTION_MODE);
		LoadIdentity();

		Math::Matrix4 orthoMat = Math::Ortho(0.0f, m_RenderData.Width, m_RenderData.Height, 0.0f, -1.0f, 1.0f);
		*m_CurrentMatrix = orthoMat * (*m_CurrentMatrix);

		MatrixMode(MODELVIEW_MODE);
		LoadIdentity();
	}

	void GLRenderer::UpdateViewport()
	{
		glViewport(0, 0, m_RenderData.Width, m_RenderData.Height);
	}

	void GLRenderer::MatrixMode(int mode)
	{
		if (mode == PROJECTION_MODE) m_CurrentMatrix = &m_Projection;
		if (mode == MODELVIEW_MODE) m_CurrentMatrix = &m_ModelView;
		m_MatrixMode = mode;
	}

	void GLRenderer::LoadIdentity()
	{
		if (m_CurrentMatrix)
		{
			*m_CurrentMatrix = Math::Matrix4::Identity();
		}
	}

	void GLRenderer::BeginDrawing()
	{
		BASIC_RUNTIME_CHECK(m_PlatformInst != nullptr, "GLRenderer instance need a platform instance");

		m_PlatformInst->TimeData().Current = m_PlatformInst->GetTime();
		m_PlatformInst->TimeData().Update = m_PlatformInst->TimeData().Current - m_PlatformInst->TimeData().Previous;
		m_PlatformInst->TimeData().Previous = m_PlatformInst->TimeData().Current;

		//TODO: 是否需要做一些矩阵变换呢？
	}

	void GLRenderer::EndDrawing()
	{
		DrawRenderBatch();
		BASIC_RUNTIME_CHECK(m_PlatformInst != nullptr, "GLRenderer instance need a platform instance");
		m_PlatformInst->SwapBuffers();

		m_PlatformInst->TimeData().Current = m_PlatformInst->GetTime();
		m_PlatformInst->TimeData().Draw = m_PlatformInst->TimeData().Current - m_PlatformInst->TimeData().Previous;
		m_PlatformInst->TimeData().Previous = m_PlatformInst->TimeData().Current;
		
		// TODO: 检查是否需要延长帧时间
		m_PlatformInst->PollInputEvents();
		m_PlatformInst->TimeData().FrameCounter++;
	}

	void GLRenderer::DrawTriangle(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color)
	{
		//BeginVertexInput(TRIANGLES);
		//{
		//	ColorV(color);
		//	Vertex3f(v1.x, v1.y, v1.z);
		//	Vertex3f(v2.x, v2.y, v2.z);
		//	Vertex3f(v3.x, v3.y, v3.z);
		//}
		//EndVertexInput();
		BeginVertexInput(QUADS);
		{
			ColorV(color);
			Vertex3f(v1.x, v1.y, v1.z);
			Vertex3f(v2.x, v2.y, v2.z);
			Vertex3f(v3.x, v3.y, v3.z);
			Vertex3f(v1.x, v1.y, v1.z);
		}
		EndVertexInput();
	}

	void GLRenderer::DrawTriangle(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color)
	{
		//BeginVertexInput(TRIANGLES);
		//{
		//	ColorV(color);
		//	Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
		//	Vertex3f(v2.x, v2.y, m_Batch.CurrentDepth);
		//	Vertex3f(v3.x, v3.y, m_Batch.CurrentDepth);
		//}
		//EndVertexInput();
		BeginVertexInput(QUADS);
		{
			ColorV(color);
			Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
			Vertex3f(v2.x, v2.y, m_Batch.CurrentDepth);
			Vertex3f(v3.x, v3.y, m_Batch.CurrentDepth);
			Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
		}
		EndVertexInput();
	}

	void GLRenderer::DrawTriangleGradients(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3)
	{
		//BeginVertexInput(TRIANGLES);
		//{
		//	ColorV(color1);
		//	Vertex3f(v1.x, v1.y, v1.z);
		//	ColorV(color2);
		//	Vertex3f(v2.x, v2.y, v2.z);
		//	ColorV(color3);
		//	Vertex3f(v3.x, v3.y, v3.z);
		//}
		//EndVertexInput();
		BeginVertexInput(QUADS);
		{
			ColorV(color1);
			Vertex3f(v1.x, v1.y, v1.z);
			ColorV(color2);
			Vertex3f(v2.x, v2.y, v2.z);
			ColorV(color3);
			Vertex3f(v3.x, v3.y, v3.z);
			ColorV(color1);
			Vertex3f(v1.x, v1.y, v1.z);
		}
		EndVertexInput();
	}

	void GLRenderer::DrawTriangleGradients(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3)
	{
		//BeginVertexInput(TRIANGLES);
		//{
		//	ColorV(color1);
		//	Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
		//	ColorV(color2);
		//	Vertex3f(v2.x, v2.y, m_Batch.CurrentDepth);
		//	ColorV(color3);
		//	Vertex3f(v3.x, v3.y, m_Batch.CurrentDepth);
		//}
		//EndVertexInput();
		BeginVertexInput(QUADS);
		{
			ColorV(color1);
			Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
			ColorV(color2);
			Vertex3f(v2.x, v2.y, m_Batch.CurrentDepth);
			ColorV(color3);
			Vertex3f(v3.x, v3.y, m_Batch.CurrentDepth);
			ColorV(color1);
			Vertex3f(v1.x, v1.y, m_Batch.CurrentDepth);
		}
		EndVertexInput();
	}

	GLRenderer::GLRenderer()
	{

	}

	void GLRenderer::LoadRenderBatch()
	{
		// TODO: 未来可以做成从配置文件读取RenderBatch的各种参数，而不是直接写死在代码里。

		//-------------------------------------------------------------------------------
		// Init Vertex Buffers
		//-------------------------------------------------------------------------------
		m_Batch.Buffers.resize(m_Batch.BufferCount);
		for (int i = 0; i < m_Batch.BufferCount; i++)
		{
			auto& buffer = m_Batch.Buffers[i];
			
			glCreateVertexArrays(1, &buffer.VertexArray);
			glBindVertexArray(buffer.VertexArray);

			glCreateBuffers(1, &buffer.PositionBuffer);
			buffer.PositionData.resize(buffer.BufferElements * buffer.ElementVertice * 3);
			std::fill(buffer.PositionData.begin(), buffer.PositionData.end(), 0.0f);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.PositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, buffer.PositionData.size() * sizeof(float), reinterpret_cast<const void*>(buffer.PositionData.data()), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(ATTRIB_VERTEX_POSITION_DEFAULT_LOC);
			glVertexAttribPointer(ATTRIB_VERTEX_POSITION_DEFAULT_LOC, 3, GL_FLOAT, 0, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glCreateBuffers(1, &buffer.TexCoordBuffer);
			buffer.TexCoordData.resize(buffer.BufferElements * buffer.ElementVertice * 2);
			std::fill(buffer.TexCoordData.begin(), buffer.TexCoordData.end(), 0.0f);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.TexCoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, buffer.TexCoordData.size() * sizeof(float), reinterpret_cast<const void*>(buffer.TexCoordData.data()), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(ATTRIB_VERTEX_TEXCOORD_DEFAULT_LOC);
			glVertexAttribPointer(ATTRIB_VERTEX_TEXCOORD_DEFAULT_LOC, 2, GL_FLOAT, 0, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glCreateBuffers(1, &buffer.NormalBuffer);
			buffer.NormalData.resize(buffer.BufferElements * buffer.ElementVertice * 3);
			std::fill(buffer.NormalData.begin(), buffer.NormalData.end(), 0.0f);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.NormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, buffer.NormalData.size() * sizeof(float), reinterpret_cast<const void*>(buffer.NormalData.data()), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(ATTRIB_VERTEX_NORMAL_DEFAULT_LOC);
			glVertexAttribPointer(ATTRIB_VERTEX_NORMAL_DEFAULT_LOC, 3, GL_FLOAT, 0, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glCreateBuffers(1, &buffer.ColorBuffer);
			buffer.ColorData.resize(buffer.BufferElements * buffer.ElementVertice * 4);
			std::fill(buffer.ColorData.begin(), buffer.ColorData.end(), 0);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.ColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, buffer.ColorData.size() * sizeof(unsigned char), reinterpret_cast<const void*>(buffer.ColorData.data()), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(ATTRIB_VERTEX_COLOR_DEFAULT_LOC);
			// NOTE: 这里归一化需要设置为GL_TRUE
			glVertexAttribPointer(ATTRIB_VERTEX_COLOR_DEFAULT_LOC, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glCreateBuffers(1, &buffer.IndexBuffer);
			buffer.IndexData.resize(buffer.BufferElements * 6);
			for (int i = 0; i < buffer.BufferElements; i++)
			{
				buffer.IndexData[i * 6 + 0] = i * 4 + 0;
				buffer.IndexData[i * 6 + 1] = i * 4 + 1;
				buffer.IndexData[i * 6 + 2] = i * 4 + 2;
				buffer.IndexData[i * 6 + 3] = i * 4 + 0;
				buffer.IndexData[i * 6 + 4] = i * 4 + 2;
				buffer.IndexData[i * 6 + 5] = i * 4 + 3;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.IndexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.BufferElements * 6 * sizeof(unsigned int), reinterpret_cast<const void*>(buffer.IndexData.data()), GL_STATIC_DRAW);

			glBindVertexArray(0);
		}

		//-------------------------------------------------------------------------------
		// Init Vertex DrawCalls
		//-------------------------------------------------------------------------------
		m_Batch.Draws.resize(m_Batch.DrawCount);
		for (int i = 0; i < m_Batch.DrawCount; i++)
		{
			auto& draw = m_Batch.Draws[i];
			draw.Mode = QUADS;
			draw.Texture = 0;
			draw.VertexAlignment = 0;
			draw.VertexCounter = 0;
		}
	}
	void GLRenderer::DrawRenderBatch()
	{
		// NOTE：准备绘制的数据
		if (m_Batch.VertexCounter > 0)
		{
			auto& currentBuffer = m_Batch.Buffers[m_Batch.CurrentBuffer];

			glBindVertexArray(currentBuffer.VertexArray);

			glBindBuffer(GL_ARRAY_BUFFER, currentBuffer.PositionBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Batch.VertexCounter * 3 * sizeof(float), currentBuffer.PositionData.data());

			glBindBuffer(GL_ARRAY_BUFFER, currentBuffer.TexCoordBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Batch.VertexCounter * 2 * sizeof(float), currentBuffer.TexCoordData.data());

			glBindBuffer(GL_ARRAY_BUFFER, currentBuffer.NormalBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Batch.VertexCounter * 3 * sizeof(float), currentBuffer.NormalData.data());

			glBindBuffer(GL_ARRAY_BUFFER, currentBuffer.ColorBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_Batch.VertexCounter * 4 * sizeof(unsigned char), currentBuffer.ColorData.data());

			// NOTE: 如果初始化处调用glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)函数了，则这里一定要将glBindBuffer函数绑定好，推荐在初始化处直接就绑定好

			glBindVertexArray(0);
		}

		Math::Matrix4 mvp = m_Projection * m_ModelView;

		if (m_Batch.VertexCounter > 0)
		{
			// NOTE: 开始绘制
			glBindVertexArray(m_Batch.Buffers[m_Batch.CurrentBuffer].VertexArray);
			m_CurrentShader->Bind();

			m_CurrentShader->SetUniformMat4("mvp", mvp);
			
			for (int i = 0, vertexOffset = 0; i < m_Batch.DrawCounter; i++)
			{
				if (m_Batch.Draws[i].Mode == TRIANGLES || m_Batch.Draws[i].Mode == LINES)
				{
					glDrawArrays(m_Batch.Draws[i].Mode, vertexOffset, m_Batch.Draws[i].VertexCounter);
				}
				else if (m_Batch.Draws[i].Mode == QUADS)
				{
					int indices = m_Batch.Draws[i].VertexCounter / 4 * 6;
					int offset = vertexOffset / 4 * 6 * sizeof(GLuint);
					glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, (GLvoid*)(offset));
				}
				vertexOffset += (m_Batch.Draws[i].VertexCounter + m_Batch.Draws[i].VertexAlignment);
			}

			m_CurrentShader->UnBind();
			glBindVertexArray(0);

			m_Batch.VertexCounter = 0;
			m_Batch.DrawCounter = 1;
			m_Batch.CurrentDepth = -1.0f;
			for (int i = 0; i < m_Batch.Draws.size(); i++)
			{
				m_Batch.Draws[i].Mode = QUADS;
				m_Batch.Draws[i].Texture = 0;
				m_Batch.Draws[i].VertexCounter = 0;
				m_Batch.Draws[i].VertexAlignment = 0;
			}
		}
	}

	bool GLRenderer::CheckRenderBatchLimit(int vCount)
	{
		bool overflow = false;

		if ((vCount + m_Batch.VertexCounter) >= m_Batch.Buffers[m_Batch.CurrentBuffer].BufferElements * 4)
		{
			overflow = true;

			DrawCall curDraw = m_Batch.Draws[m_Batch.DrawCounter - 1];
			curDraw.VertexAlignment = 0;
			curDraw.VertexCounter = 0;

			DrawRenderBatch();

			m_Batch.Draws[m_Batch.DrawCounter - 1] = curDraw;
		}

		return overflow;
	}

	void GLRenderer::BeginVertexInput(int mode)
	{
		DrawCall* curDraw = &m_Batch.Draws[m_Batch.DrawCounter - 1];
		DrawCall* nextDraw = nullptr;
		if (mode != curDraw->Mode)
		{
			if (curDraw->VertexCounter > 0)
			{
				if (curDraw->Mode == LINES || curDraw->Mode == TRIANGLES)
				{
					int remainder = curDraw->VertexCounter % 4;
					if (remainder > 0)
					{
						curDraw->VertexAlignment = 4 - remainder;
					}
					else if(remainder == 0)
					{
						curDraw->VertexAlignment = 0;
					}
				}
				else if (curDraw->Mode == QUADS)
				{
					curDraw->VertexAlignment = 0;
				}

				if (!CheckRenderBatchLimit(curDraw->VertexAlignment))
				{
					m_Batch.VertexCounter += curDraw->VertexAlignment;
					m_Batch.DrawCounter++;
				}
			}

			nextDraw = &m_Batch.Draws[m_Batch.DrawCounter - 1];
			nextDraw->Mode = mode;
			nextDraw->Texture = 0;
			nextDraw->VertexAlignment = 0;
			nextDraw->VertexCounter = 0;
		}
	}

	void GLRenderer::EndVertexInput()
	{
		m_Batch.CurrentDepth += 0.00001;
	}

	void GLRenderer::ColorV(Graphics::Color color)
	{
		m_CurrentColor = color;
	}

	void GLRenderer::Vertex3f(float x, float y, float z)
	{
		float tx = x;
		float ty = y;
		float tz = z;
		Math::Vector4 coord(tx, ty, tz, 1.0f);

		if (m_TransformRequired)
		{
			coord = m_Transform * coord;
		}

		
		if (m_Batch.VertexCounter > (m_Batch.Buffers[m_Batch.CurrentBuffer].BufferElements * 4 - 4))
		{
			if (m_Batch.Draws[m_Batch.DrawCounter - 1].VertexCounter % 2 == 0 && m_Batch.Draws[m_Batch.DrawCounter - 1].Mode == LINES)
			{
				CheckRenderBatchLimit(2 + 1);
			}
			if (m_Batch.Draws[m_Batch.DrawCounter - 1].VertexCounter % 3 == 0 && m_Batch.Draws[m_Batch.DrawCounter - 1].Mode == TRIANGLES)
			{
				CheckRenderBatchLimit(3 + 1);
			}
			if (m_Batch.Draws[m_Batch.DrawCounter - 1].VertexCounter % 4 == 0 && m_Batch.Draws[m_Batch.DrawCounter - 1].Mode == QUADS)
			{
				CheckRenderBatchLimit(4 + 1);
			}
		}

		// Position Data
		m_Batch.Buffers[m_Batch.CurrentBuffer].PositionData[m_Batch.VertexCounter * 3] = tx;
		m_Batch.Buffers[m_Batch.CurrentBuffer].PositionData[m_Batch.VertexCounter * 3 + 1] = ty;
		m_Batch.Buffers[m_Batch.CurrentBuffer].PositionData[m_Batch.VertexCounter * 3 + 2] = tz;

		// Tex Coord Data
		m_Batch.Buffers[m_Batch.CurrentBuffer].TexCoordData[m_Batch.VertexCounter * 2] = m_TexCoord.x;
		m_Batch.Buffers[m_Batch.CurrentBuffer].TexCoordData[m_Batch.VertexCounter * 2 + 1] = m_TexCoord.y;

		// Normal Data
		m_Batch.Buffers[m_Batch.CurrentBuffer].NormalData[m_Batch.VertexCounter * 3] = m_Normal.x;
		m_Batch.Buffers[m_Batch.CurrentBuffer].NormalData[m_Batch.VertexCounter * 3 + 1] = m_Normal.y;
		m_Batch.Buffers[m_Batch.CurrentBuffer].NormalData[m_Batch.VertexCounter * 3 + 2] = m_Normal.z;

		// Color Data
		m_Batch.Buffers[m_Batch.CurrentBuffer].ColorData[m_Batch.VertexCounter * 4] = m_CurrentColor.r;
		m_Batch.Buffers[m_Batch.CurrentBuffer].ColorData[m_Batch.VertexCounter * 4 + 1] = m_CurrentColor.g;
		m_Batch.Buffers[m_Batch.CurrentBuffer].ColorData[m_Batch.VertexCounter * 4 + 2] = m_CurrentColor.b;
		m_Batch.Buffers[m_Batch.CurrentBuffer].ColorData[m_Batch.VertexCounter * 4 + 3] = m_CurrentColor.a;

		m_Batch.Draws[m_Batch.DrawCounter - 1].VertexCounter++;
		m_Batch.VertexCounter++;
	}

	void GLRenderer::Vertex2f(float x, float y)
	{
		Vertex3f(x, y, m_Batch.CurrentDepth);
	}
}
}