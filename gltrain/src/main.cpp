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

void errorCallback(int error_code, const char* description)
{
	GLSimpleLogger::GetInstance().Error("Error: [Code: {}], [Description:{}]\n", error_code, description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main()
{
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Title", nullptr, nullptr);
	if (!window) return -1;

	glfwSetKeyCallback(window, keyCallback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// OpenGL global option
	glViewport(0, 0, 1280, 720);

	GLShader defaultShader;
	std::string vsCode, fsCode;
	defaultShader.LoadShaderFromFile();
	defaultShader.Bind();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -100.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280 / (float)720, 0.1f, 1000.0f);
	glm::mat4 mvp = projection * view * model;
	glUniformMatrix4fv(defaultShader.m_Locations["mvp"], 1, GL_FALSE, glm::value_ptr(mvp));
	defaultShader.UnBind();

	GLRenderBatch batch;
	batch.LoadRenderBatch(5, 1024);
	batch.m_CurrentBufferIdx = 0;
	batch.m_CurrentShaderId = defaultShader.ProgramID();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		batch.DrawTriangle(glm::vec3(-10.5, -10.5, 0.0),
			glm::vec3(10.5, -10.5, 0.0),
			glm::vec3(0.0, 10.5, 0.0),
			glm::vec4(1.0, 0.0, 0.0, 1.0));

		batch.DrawTriangle(glm::vec3(-10.5 + 20, -10.5 + 20, 0.0),
			glm::vec3(10.5 + 20, -10.5 + 20, 0.0),
			glm::vec3(0.0 + 20, 10.5 + 20, 0.0),
			glm::vec4(1.0, 0.0, 1.0, 1.0));

		batch.DrawLine(glm::vec3(-50.5, -10.5 + 30, 0.0),
			glm::vec3(50.5, -10.5 + 30, 0.0), 
			glm::vec4(0.0, 1.0, 1.0, 1.0));
		// TODO: 手动绘制，后边应该改成自动绘制
		batch.DrawRenderBatch();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}