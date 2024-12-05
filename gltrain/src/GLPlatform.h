#pragma once

#include <mutex>
#include <memory>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Casic
{
namespace GL
{
	class GLPlatform {
	public:
		typedef struct WindowData{
			std::string Title;
			int Width;
			int Height;
		}WindowData;
		typedef struct Time {
			float Previous = 0.0f;
			float Current = 0.0f;
			float Update = 0.0f;
			float Draw = 0.0f;
			float Frame = 0.0f;
			float Target = 0.0f;
			unsigned long long FrameCounter = 0;
		}Time;

	public:
		static GLPlatform& GetInstance()
		{
			std::call_once(s_InitFlag, []() {
				s_Instance.reset(new GLPlatform());
			});
			return *s_Instance;
		}

		void InitPlatform();
		bool WindowShouldClose();
		void ShutdownPlatform();
		void SwapBuffers();
		void PollInputEvents();
		void SetWindowData(std::string title, int width, int height);

		static float GetTime();
		static void ErrorCallback(int error_code, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned int codepoint);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		~GLPlatform();

		GLFWwindow* Window();
		WindowData& GetWindowData();
		Time& TimeData();

	private:
		GLPlatform(WindowData wd = { "Casic GL Window", 1280, 720 });

	private:
		WindowData m_WindowData;
		GLFWwindow* m_Window;
		Time m_TimeData;

	private:
		static std::once_flag s_InitFlag;
		static std::unique_ptr<GLPlatform> s_Instance;
	};
}
}