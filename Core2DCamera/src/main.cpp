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

	float rotation = 0.0;

	platform.DisableCursor();
	Math::Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	Graphics::Rectangle player = { 400, screenHeight / 2 - 40, 40, 40 };
	Graphics::Rectangle buildings[100] = { 0 };
	Graphics::Color buildColors[100] = {  };

	int spacing = 0;

	for (int i = 0; i < 100; i++)
	{
		buildings[i].width = (float)Util::RandomInt(50, 200);
		buildings[i].height = (float)Util::RandomInt(100, 800);
		buildings[i].y = screenHeight / 2 - buildings[i].height;
		buildings[i].x = -6000.0f + spacing;

		spacing += (int)buildings[i].width;

		buildColors[i] = { static_cast<unsigned char>(Util::RandomInt(200, 240)), static_cast<unsigned char>(Util::RandomInt(200, 240)), static_cast<unsigned char>(Util::RandomInt(200, 250)), 255 };
	}

	Camera2D camera2d{  };
	camera2d.target = { player.x + 20.0f, player.y + 20.0f };
	camera2d.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
	camera2d.rotation = 0.0f;
	camera2d.zoom = 1.0f;

	while (!platform.WindowShouldClose())
	{
		renderer.ClearColorBuffer({ 255, 255, 255, 255 });

		//--------------------------------------------------------------------------------------------------------------------------
		// update 
		//--------------------------------------------------------------------------------------------------------------------------
		if (input.IsKeyDown(KEY_RIGHT)) player.x += 5.0f;
		if (input.IsKeyDown(KEY_LEFT)) player.x -= 5.0f;
		if (input.IsKeyDown(KEY_Q)) camera2d.rotation += 1.0f;
		if (input.IsKeyDown(KEY_E)) camera2d.rotation -= 1.0f;
		if (input.IsKeyPressed(KEY_Z))
		{
			player = { 400, (float)screenHeight / 2 - 40, 40, 40 };
			camera2d.target = { player.x + 20.0f, player.y + 20.0f };
			camera2d.rotation = 0.0f;
			camera2d.zoom = 1.0f;
		}
		camera2d.target = { player.x + 20.0f, player.y + 20.0f };

		//--------------------------------------------------------------------------------------------------------------------------
		// draw 
		//--------------------------------------------------------------------------------------------------------------------------
		renderer.BeginDrawing();
		{
			renderer.BeginMode2D(camera2d);
			{
				renderer.DrawRectangle(-6000, screenHeight / 2, 13000, 8000, DARKGRAY);

				for (int i = 0; i < 100; i++) renderer.DrawRectangle(buildings[i].x, buildings[i].y, buildings[i].width, buildings[i].height, buildColors[i]);

				renderer.DrawRectangle(player.x, player.y, player.width, player.height, RED);
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