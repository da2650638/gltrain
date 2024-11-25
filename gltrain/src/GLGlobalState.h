#pragma once

#include <mutex>
#include <memory>
#include <unordered_map>

#include "GLSimpleLogger.h"
#include "GLInput.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <yaml-cpp/yaml.h>

class GLRenderBatch;

class GLGlobalState {
public:
	// Glfw context
	typedef struct WindowData {
		int Width;
		int Height;
		std::string Title;
	};
	static void errorCallback(int error_code, const char* description);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void InitPlatform(const WindowData& wd = {1280, 720, "default title"});
	static bool WindowShouldClose();
	static void ShutdownPlatform();

	// shader configs
	typedef struct LocationData {
		std::string Name;
		int DefaultLoc;
		unsigned int IndexInLocArray;
		std::string Type;
		std::string LocationType;
	}LocationData;
	typedef struct ShaderInfo {
		std::string VertexShaderFile;
		std::string FragmentShaderFile;
		std::unordered_map<std::string, LocationData> LocationDatas;
	}ShaderInfo;
	ShaderInfo LoadShaderInfos(const std::string& filename, const std::string& shaderName);

	// time data
	// TODO: 时间数据结构的成员还得拓展
	typedef struct Time {
		float Previous = 0.0f;
		float Current = 0.0f;
		float Update = 0.0f;
		float Draw = 0.0f;
		float Frame = 0.0f;
		unsigned long long FrameCounter;
	}Time;
	static float GetTime() { return glfwGetTime(); }

	static void PollInputEvent();

	static void BeginDrawing();
	static void EndDrawing(GLRenderBatch* activeBatch);

	static GLGlobalState& GetInstance()
	{
		std::call_once(m_InitFlag, []() {
			m_Instance.reset( new GLGlobalState() );
		});
		return *m_Instance;
	}
	~GLGlobalState() {}

public:
	// 全局数据结构
	static Time s_Time;
	static WindowData s_WindowData;
	static GLFWwindow* s_Window;

private:
	GLGlobalState() {}
	GLGlobalState(const GLGlobalState&) = delete;
	GLGlobalState& operator=(const GLGlobalState&) = delete;

	static std::once_flag m_InitFlag;
	static std::unique_ptr<GLGlobalState> m_Instance;
};