#include <iostream>
#include <format>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLPlatform.h"
#include "GLInput.h"
#include "SimpleLogger.h"
#include "GLRenderer.h"

#include "Casic/CasicMatrixTransform.h"
#include "Casic/CasicGraphics.h"

using namespace Casic;
using namespace Casic::GL;

const char* vertexShaderSrc =
"#version 460 core\n"
"layout(location = 0) in vec3 vertexPosition;\n"
"layout(location = 1) in vec2 vertexTexcoord;\n"
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

	auto& renderer = GLRenderer::GetInstance();
	renderer.SetPlatform(&platform);
	renderer.SetupViewport(platform.GetWindowData().Width, platform.GetWindowData().Height);
	renderer.SetTargetFps(60);

	auto& input = GLInput::GetInstance();

	// shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);
	int vertexShaderCompileStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompileStatus);
	if (vertexShaderCompileStatus == GL_FALSE) 
	{
		int length = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length + 1];
		memset(log, 0, length + 1);
		glGetShaderInfoLog(vertexShader, length + 1, nullptr, log);
		std::cout << std::format("vertex shader:[{}], compile error: {}\n", vertexShader, log);
		delete [] log;
		return 1;
	}
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);
	int fragmentShaderCompileStatus = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS , &fragmentShaderCompileStatus);
	if (fragmentShaderCompileStatus == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length + 1];
		memset(log, 0, length + 1);
		glGetShaderInfoLog(fragmentShader, length, nullptr, log);
		std::cout << std::format("fragment shader:[{}], compile error: {}\n", fragmentShader, log);
		delete [] log;
		return 1;
	}
	int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	int linkStatus = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length + 1];
		memset(log, 0, length + 1);
		glGetProgramInfoLog(program, length, nullptr, log);
		std::cout << std::format("shader program:[{}], link error: {}\n", program, log);
		delete[] log;
		return 1;
	}
	// ÉèÖÃshaderµÄuniform
	glUseProgram(program);
	Math::Matrix4 projection = Math::Ortho(0.0f, 1280.0f, 720.0f, 0.0f, 1.0f, -1.0f);
	Math::Matrix4 view = Math::LookAt({ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	Math::Matrix4 model = Math::Scale({ 1.0f, 1.0f, 1.0f });
	Math::Matrix4 mvp = projection * view * model;
	int mvpLocation = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, Graphics::ToOpenGLMatrix4(mvp).v);

	// vertex buffer
	unsigned int vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	float vertexPositions[] = {
		600.f, 400.f, 0.0f,
		680.f, 400.f, 0.0f,
		680.f, 320.f, 0.0f,

		600.f, 400.f, 0.0f,
		680.f, 320.f, 0.0f,
		600.f, 320.f, 0.0f
	};
	unsigned int posVbo;
	glCreateBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float vertexColors[] = {
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,

		1.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0
	};
	unsigned int colVbo;
	glCreateBuffers(1, &colVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, 0, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
	glBindVertexArray(0);

	while (!platform.WindowShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		platform.SwapBuffers();
		platform.PollInputEvents();
	}

	platform.ShutdownPlatform();
}