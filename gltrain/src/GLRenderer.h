#pragma once

#include <mutex>
#include <memory>
#include <stack>

#include "GLPlatform.h"
#include "GLGlobal.h"
#include "GLShader.h"
#include "GLRenderData.h"

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

		void DrawTriangle(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color);
		void DrawTriangle(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color);
		void DrawTriangleGradients(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3);
		void DrawTriangleGradients(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3);
	private:
		GLRenderer();

		void LoadRenderBatch();
		void DrawRenderBatch();
		bool CheckRenderBatchLimit(int vCount);

		void BeginVertexInput(int mode);
		void EndVertexInput();
		void ColorV(Graphics::Color color);
		void Vertex3f(float x, float y, float z);
		void Vertex2f(float x, float y);

	private:
		// TODO: 这种设计能否改进呢？
		// NOTE: 任何使用m_PlatformInst必须检查是否合法并提示编译错误或抛出异常
		GLPlatform* m_PlatformInst;
		RenderData m_RenderData;
		//-------------------------
		// 全局状态
		//-------------------------
		Graphics::Color m_CurrentColor{ 255, 255, 255, 255 };
		Math::Vector2 m_TexCoord = { 0.0f, 0.0f };
		Math::Vector3 m_Normal = {0.0f, 0.0f, 0.0f};

		//-------------------------
		// Shader相关成员
		//-------------------------
		GLShader m_DefaultShader;
		GLShader* m_CurrentShader = nullptr;
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
		//-------------------------
		// Render Batch相关成员
		//-------------------------
		RenderBatch m_Batch;

	private:
		static std::once_flag m_InitFlag;
		static std::unique_ptr<GLRenderer> m_Instance;
	};
}
}