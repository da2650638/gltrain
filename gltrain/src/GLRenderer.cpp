#include "GLRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		//---------------------------------------------------------------
		// TODO: 初始化RenderBatch
		//---------------------------------------------------------------

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

		Math::Matrix4 orthoMat = Math::Ortho(0.0f, m_RenderData.Width, m_RenderData.Height, 0.0f, 0.0f, 1.0f);
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

	}

	GLRenderer::GLRenderer()
	{
	}
}
}