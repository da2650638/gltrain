#include <iostream>
#include <format>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLGlobal.h"
#include "GLVertexBuffer.h"
#include "GLRenderBatch.h"
#include "GLShader.h"


int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1280;
	const int screenHeight = 720;

	GLGlobalState::GetInstance();
	GLGlobalState::InitPlatform({ screenWidth, screenHeight, "basic shape" });

	GLShader defaultShader;
	std::string vsCode, fsCode;
	defaultShader.LoadShaderFromFile();
	defaultShader.Bind();
	glm::mat4 mvp = glm::ortho(0.0f, (float)GLGlobalState::s_WindowData.Width, (float)GLGlobalState::s_WindowData.Height, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(defaultShader.m_Locations["mvp"], 1, GL_FALSE, glm::value_ptr(mvp));
	defaultShader.UnBind();

	GLRenderBatch batch;
	batch.LoadRenderBatch(5, 1024);
	batch.m_CurrentBufferIdx = 0;
	batch.m_CurrentShaderId = defaultShader.ProgramID();

	float rotation = 0.0f;

	while (!GLGlobalState::WindowShouldClose())
	{
		GLGlobalState::BeginDrawing();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		rotation += 0.2f;

		//batch.DrawLine(glm::vec3(0, 360.0f, 0.0f),
		//	glm::vec3(1280.0f, 360.0f, 0.0),
		//	glm::vec4(0.0, 1.0, 1.0, 1.0));

		batch.DrawTriangle(
			{ screenWidth / 4.0f * 3.0f, 80.0f },
			{ screenWidth / 4.0f * 3.0f - 60.0f, 150.0f},
			{ screenWidth / 4.0f * 3.0f + 60.0f, 150.0f},
			glm::vec4(135 / 255.f, 60 / 255.f, 190 / 255.f, 255 / 255.f)
		);
		batch.DrawTriangleLines(
			{ screenWidth / 4.0f * 3.0f, 160.0f }, 
			{ screenWidth / 4.0f * 3.0f - 20.0f, 230.0f },
			{ screenWidth / 4.0f * 3.0f + 20.0f, 230.0 }, 
			{ 0 / 255.f, 82 / 255.f, 172 / 255.f, 255 / 255.f }
		);

		//rotation += 1.0f;
		//// Rectangle shapes and lines
		batch.DrawRectangle(screenWidth / 4 * 2 - 60, 100, 120, 60, { 230 /255.f, 41 / 255.f, 55 / 255.f, 255 /255.f });
		batch.DrawRectangleLines(screenWidth / 4 * 2 - 40, 320, 80, 60, { 255 / 255.f, 161 / 255.f, 0 / 255.f, 255 / 255.f });  // NOTE: Uses QUADS internally, not lines

		batch.DrawPoly({ screenWidth / 4.0f * 3, 330 }, 6, 80, rotation, { 127 / 255.f, 106 / 255.f, 79 / 255.f, 255 / 255.f });
		batch.DrawPolyLines({ screenWidth / 4.0f * 3, 330 }, 6, 90, rotation, { 127 / 255.f, 106 / 255.f, 79 / 255.f, 255 / 255.f });
		batch.DrawPolyLinesEx( { screenWidth / 4.0f * 3, 330 }, 6, 85, rotation, 5, { 211 / 255.f, 176 / 255.f, 131 / 255.f, 255 / 255.f });
		
		GLGlobalState::EndDrawing(&batch);
	}

	GLGlobalState::ShutdownPlatform();
}