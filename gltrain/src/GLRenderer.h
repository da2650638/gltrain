#pragma once

#include <mutex>
#include <memory>
#include <stack>

#include "GLPlatform.h"
#include "GLGlobal.h"
#include "GLShader.h"

namespace Casic
{
namespace GL
{

	class GLRenderer {
	public:
		typedef struct RenderData {
			int Width;
			int Height;
			unsigned int TargetFps = 60;
		} RenderData;

		static GLRenderer& GetInstance();

		~GLRenderer();

		void SetPlatform(GLPlatform* platform);
		void SetRenderData(RenderData rd);
		void SetRenderSize(int width, int height);
		void RenderInit();
		void SetTargetFps(unsigned int fps);
		void SetupViewport(int width, int height);
		void UpdateViewport();

		void MatrixMode(int mode);
		void LoadIdentity();

		void BeginDrawing();
		void EndDrawing();
	private:
		GLRenderer();

	private:
		// TODO: 任何使用m_PlatformInst必须检查是否合法并提示编译错误或抛出异常
		GLPlatform* m_PlatformInst;
		RenderData m_RenderData;
		//-------------------------
		// Shader相关成员
		//-------------------------
		GLShader m_DefaultShader;
		GLShader* m_CurrentShader;
		//-------------------------
		// Matrix 成员
		//-------------------------
		int m_MatrixMode;
		Math::Matrix4* m_CurrentMatrix;
		Math::Matrix4 m_Projection;
		Math::Matrix4 m_ModelView;
		Math::Matrix4 m_Transform;
		bool m_TransformRequired;
		std::stack<Math::Matrix4> m_MatStack;

	private:
		static std::once_flag m_InitFlag;
		static std::unique_ptr<GLRenderer> m_Instance;
	};
}
}