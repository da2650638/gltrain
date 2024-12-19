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
		void DrawTriangleLines(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color);
		void DrawTriangleLines(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color);
		void DrawTriangleGradients(Math::Vector3 v1, Math::Vector3 v2, Math::Vector3 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3);
		void DrawTriangleGradients(Math::Vector2 v1, Math::Vector2 v2, Math::Vector2 v3, Graphics::Color color1, Graphics::Color color2, Graphics::Color color3);
		/*
		 * DrawRectanglePro: Draws a rotated rectangle with a customizable pivot point for rotation.
		 *
		 * Diagram:
		 *
		 * The rectangle is defined in 2D space as follows:
		 *
		 *                         (topLeft) +------------------+ (topRight)
		 *                                  |                  |
		 *                                  |                  |
		 *                                  |                  |
		 *                                  |   (pivot)        |
		 *                                  |       +          |
		 *                                  |                  |
		 *                                  |                  |
		 *                     (bottomLeft) +------------------+ (bottomRight)
		 *  Key Relationships:
		 *    - `pivot`: Defines the rotation center relative to the rectangle's top-left corner.
		 *    - `x, y`: The position of the `pivot` in **world space**.
		 *    - `topLeft`: Calculated based on the pivot's position in world space and its relative offset.
		 *
		 * Args:
		 *   x (int): X-coordinate of the pivot point in world space.
		 *   y (int): Y-coordinate of the pivot point in world space.
		 *   width (int): The width of the rectangle.
		 *   height (int): The height of the rectangle.
		 *   rotation (float): The rotation angle in degrees, applied around the `pivot`.
		 *   color (Graphics::Color): The fill color of the rectangle.
		 *   pivot (Math::Vector2): The relative pivot point within the rectangle bounds for rotation.
		 *
		 *  Example:
		 *    DrawRectanglePro(100, 100, 50, 30, 45.0f, Color(255, 0, 0, 255), {25, 15});
		 *    - Rectangle's size: 50 (width) x 30 (height).
		 *    - `pivot`: At the center of the rectangle ({25, 15} relative to its bounds).
		 *    - Rotation center in world space: (100, 100).
		 *    - The top-left corner of the rectangle in world space:
		 *      (100 - 25, 100 - 15) = (75, 85).
		 * 
		 * Relationships between `pivot` and vertices:
		 *   - `topLeft = (x - pivot.x, y - pivot.y)`
		 *   - `topRight = topLeft + (width, 0)`
		 *   - `bottomRight = topLeft + (width, height)`
		 *   - `bottomLeft = topLeft + (0, height)`
		 *
		 * Note:
		 *   - The rectangle rotates around the `pivot`, which is specified in local coordinates relative to the rectangle.
		 *   - `pivot = (0, 0)` means rotation occurs around the top-left corner.
		 *   - `pivot = (width / 2, height / 2)` means rotation occurs around the rectangle's center.
		 *   - Rotation is applied if `rotation` is not zero.
		 */
		void DrawRectanglePro(int x, int y, int width, int height, float rotation, Graphics::Color color, Math::Vector2 pivot = { 0.0f, 0.0f });
		void DrawRectangleLinesPro(int x, int y, int width, int height, float rotation, Graphics::Color color, Math::Vector2 pivot = { 0.0f, 0.0f });
		void DrawRectangleProV(Math::Vector2 pos, Math::Vector2 size, float rotation, Graphics::Color color, Math::Vector2 pivot = { 0.0f, 0.0f });
		void DrawRectangleLinesProV(Math::Vector2 pos, Math::Vector2 size, float rotation, Graphics::Color color, Math::Vector2 pivot = { 0.0f, 0.0f });
		void DrawRectangle(int x, int y, int width, int height, Graphics::Color color);
		void DrawRectangleV(Math::Vector2 pos, Math::Vector2 size, Graphics::Color color);
		void DrawRectangleLines(int x, int y, int width, int height, Graphics::Color color);
		void DrawRectangleLinesV(Math::Vector2 pos, Math::Vector2 size, Graphics::Color color);
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
		void Vertex3f(Math::Vector3 vec);
		void Vertex2f(Math::Vector2 vec);

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