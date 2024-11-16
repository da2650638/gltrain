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
	GLGlobalState::GetInstance();
	GLGlobalState::InitPlatform();

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

	while (!GLGlobalState::WindowShouldClose())
	{
		GLGlobalState::BeginDrawing();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		batch.DrawLine(glm::vec3(0, 360.0f, 0.0f),
			glm::vec3(1280.0f, 360.0f, 0.0),
			glm::vec4(0.0, 1.0, 1.0, 1.0));

		batch.DrawTriangle(glm::vec3(1280.0f / 2.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 720.0f, 0.0f),
			glm::vec3(1280.0f, 720.0f, 0.0f),
			glm::vec4(1.0, 0.0, 0.0, 1.0));

		batch.DrawRectangleTemp(glm::vec3(960.0f, 0.0f, 0.0f), 320.0f, 180.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		//batch.DrawTriangle(glm::vec3(-10.5 + 20, -10.5 + 20, 0.0),
		//	glm::vec3(10.5 + 20, -10.5 + 20, 0.0),
		//	glm::vec3(0.0 + 20, 10.5 + 20, 0.0),
		//	glm::vec4(1.0, 0.0, 1.0, 1.0));
		
		GLGlobalState::EndDrawing(&batch);
	}

	GLGlobalState::ShutdownPlatform();
}