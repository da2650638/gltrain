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
		typedef struct {
			std::string Title;
			int Width;
			int Height;
		}WindowData;

	public:
		static GLPlatform& GetInstance()
		{
			std::call_once(s_InitFlag, []() {
				s_Instance.reset(new GLPlatform());
			});
			return *s_Instance;
		}

		static void InitPlatform();
		static bool WindowShouldClose();
		static void ShutdownPlatform();
		static void SwapBuffers();
		static void PollInputEvents();
		static void SetupViewport(int width, int height);

		static void SetWindowData(std::string title, int width, int height);
		static void ErrorCallback(int error_code, const char* description);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned int codepoint);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		~GLPlatform();

		static GLFWwindow* GetWindow();

	private:
		GLPlatform();

	private:
		static std::once_flag s_InitFlag;
		static std::unique_ptr<GLPlatform> s_Instance;
		static WindowData s_WindowData;
		static GLFWwindow* s_Window;
	};
}
}