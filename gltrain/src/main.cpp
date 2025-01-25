#include <iostream>
#include <format>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

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

const char* vertexShaderSrc =
"#version 460 core\n"
"layout(location = 0) in vec3 vertexPosition;\n"
"layout(location = 1) in vec2 vertexTexCoord;\n"
"layout(location = 2) in vec3 vertexNormal;\n"
"layout(location = 3) in vec4 vertexColor;\n"
"out vec4 fragColor;\n"
"uniform mat4 mvp;\n"
"void main()\n"
"{\n"
"	gl_Position = mvp * vec4(vertexPosition, 1.0);\n"
"	fragColor = vertexColor;\n"
"}\n";

const char* fragmentShaderSrc =
"#version 460 core\n"
"in vec4 fragColor;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"	outColor = fragColor;\n"
"}\n";


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

	Camera3D camera;
	camera.position = { 10.0f, 10.0f, 10.0f };												// Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };													// Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };														// Camera up vector (rotation towards target)
	camera.fovy = 45.0f;																	// Camera field-of-view Y
	camera.projection = static_cast<int>(CameraProjection::CAMERA_PERSPECTIVE);             // Camera projection type
	// NOTE: Be careful, background width must be equal or bigger than screen width
	// if not, texture should be draw more than two times for scrolling effect
	Texture2D background = LoadTexture("res/file/cyberpunk_street_background.png");
	Texture2D midground = LoadTexture("res/file/cyberpunk_street_midground.png");
	Texture2D foreground = LoadTexture("res/file/cyberpunk_street_foreground.png");

	float scrollingBack = 0.0f;
	float scrollingMid = 0.0f;
	float scrollingFore = 0.0f;
	float rotation = 0.0;

	int blendMode = 0;

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		
		//--------------------------------------------------------------------------------------------------------------------------
		// update 
		//--------------------------------------------------------------------------------------------------------------------------
		renderer.UpdateCamera(&camera, static_cast<int>(CameraMode::CAMERA_FREE));

		if (input.IsKeyPressed(KEY_SPACE))
		{
			blendMode++;
			if (blendMode >= static_cast<int>(BlendMode::GL_BLEND_COUNT))
			{
				blendMode = 0;
			}
		}

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
			//renderer.DrawTriangleLines({ 320.0f, 540.0f, 0.5f}, { 960.0f, 540.0f, 0.5f }, { 960.0f, 180.0f, 0.5f }, { 0, 255, 0, 255 });
			//renderer.DrawTriangle({ 320.0f, 540.0f}, { 960.0f, 180.0f }, { 320.0f, 180.0f }, { 255, 0, 255, 255 });
			//renderer.DrawRectangle(640, 360, 100, 100, { 255, 0, 0, 255 });
			//renderer.DrawRectangleV({ 0.0f, 0.0f }, {100.0f, 100.0f}, { 255, 255, 0, 255 });
			//renderer.DrawRectangleLinesV({ 640, 360 }, { 100, 100 }, { 255, 0, 0, 255 });
			//renderer.DrawRectangleLines(640 - 100, 360 - 100, 100, 100, { 255, 0, 0, 255 });
			
			//renderer.DrawTexture(background, screenWidth / 2 - background.width / 2, screenHeight / 2 - background.height / 2, WHITE);
			////renderer.DrawTexture(midground, screenWidth / 2 - midground.width / 2, screenHeight / 2 - midground.height / 2, WHITE);
			//renderer.BeginBlendMode(blendMode);
			//{
			//	renderer.DrawTexture(foreground, screenWidth / 2 - foreground.width / 2, screenHeight / 2 - foreground.height / 2, WHITE);
			//}
			//renderer.EndBlendMode();

			//renderer.BeginMode3D(camera);
			//{
			//	renderer.DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
			//	renderer.DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

			//	renderer.DrawGrid(10, 1.0f);
			//}
			//renderer.EndMode3D();

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