#include <iostream>
#include <format>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "GLPlatform.h"
#include "GLInput.h"
#include "SimpleLogger.h"
#include "GLRenderer.h"
#include "GLTexture.h"
#include "GLCamera.h"

#include "Casic/CasicMatrixTransform.h"
#include "Casic/CasicGraphics.h"
#include "Casic/CasicColors.h"

// TODO: 先引入glm头文件，后期使用自己的
#include <glm/gtc/matrix_inverse.hpp>

using namespace Casic;
using namespace Casic::GL;

int main()
{
	auto& platform = GLPlatform::GetInstance();
	platform.SetWindowData("My Refractor1 Window", 1280, 720);
	platform.InitPlatform();
	int screenWidth = platform.GetWindowData().Width;
	int screenHeight = platform.GetWindowData().Height;

	auto& renderer = GLRenderer::GetInstance();
	renderer.SetPlatform(&platform);   
	renderer.RenderInit();
	renderer.SetupViewport(platform.GetWindowData().Width, platform.GetWindowData().Height);
	renderer.SetTargetFps(60);

	auto& input = GLInput::GetInstance();

	Camera2D camera{};
	camera.zoom = 1.0f;

	int zoomMode = 0;	// 0--Mouse Wheel, 1--Mouse Move

	while (!platform.WindowShouldClose())
	{
		//--------------------------------------------------------------------------------------------------------------------------
		// update 
		//--------------------------------------------------------------------------------------------------------------------------
		if (input.IsKeyPressed(KEY_0)) zoomMode = 0;
		if (input.IsKeyPressed(KEY_1)) zoomMode = 1;

		// Translate based on mouse right click
		if (input.IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Math::Vector2 delta = input.GetMouseDelta();
			delta = delta * (-1.0f / camera.zoom);
			camera.target = camera.target + delta;
		}

		// Mouse Wheel处理代码，根据不同的平台可能处理方法不同，这里是Window操作系统下Glfw库的处理方法
		if (input.GetMouseWheelV() != Math::Vector2{0.0f, 0.0f})
		{
			float wheel = input.GetMouseWheelV().y;
			// Get the world point that is under the mouse
			Math::Matrix4 mat = renderer.GetCamera2DInvMatrix(camera);
			std::cout << "GetCamera2DInvMatrix:" << mat;
			Math::Vector4 mousePosVec4 = Math::Vector4(input.GetMousePosition().x, input.GetMousePosition().y, 0.0f, 1.0f);
			std::cout << "mousePosVec4:" << mousePosVec4;
			Math::Vector4 mouseWorldPosVec4 = mat * mousePosVec4;
			std::cout << "GetCamera2DInvMatrix * mousePosVec4:" << mouseWorldPosVec4;
			Math::Vector2 mouseWorldPos = Math::Vector2(mouseWorldPosVec4.x, mouseWorldPosVec4.y);

			// Set the offset to where the mouse is
			camera.offset = input.GetMousePosition();

			// Set the target to match, so that the camera maps the world space point 
			// under the cursor to the screen space point under the cursor at any zoom
			camera.target = mouseWorldPos;

			// Zoom increment
			// TODO: 搞明白这一块是什么意思？
			float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
			if (wheel < 0) scaleFactor = 1.0f / scaleFactor;
			camera.zoom = std::clamp(camera.zoom * scaleFactor, 0.125f, 64.0f);
		}

		//--------------------------------------------------------------------------------------------------------------------------
		// draw 
		//--------------------------------------------------------------------------------------------------------------------------
		renderer.BeginDrawing();
		{
			renderer.ClearColorBuffer({ 255, 255, 255, 255 });

			//renderer.DrawGridAroundZ(100, 50.0f);

			renderer.BeginMode2D(camera);
			{
				//renderer.PushMatrix();
				//{
				//	auto currentMatrix = renderer.CurrentMatrix();
				//	*currentMatrix = Math::Translate({ 0.0f, 25 * 50.0f, 0.0f }) * Math::Rotate(90.0f, { 1.0f, 0.0f, 0.0f }) * (*currentMatrix);
				//	renderer.DrawGrid(100, 50.0f);
				//}
				//renderer.PopMatrix();
				renderer.DrawGridAroundZ(100, 50.0f);

				renderer.DrawRectangle(screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2, MAROON);
			}
			renderer.EndMode2D();
		}
		renderer.EndDrawing();
		//--------------------------------------------------------------------------------------------------------------------------
		// record
		//--------------------------------------------------------------------------------------------------------------------------
		//SimpleLogger::GetInstance().Trace("PLATFORM: Frame time: {} s", GLPlatform::GetInstance().TimeData().Frame);
	}

	platform.ShutdownPlatform();
}