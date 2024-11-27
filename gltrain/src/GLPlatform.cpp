#include "GLPlatform.h"
#include "SimpleLogger.h"

namespace Casic
{
namespace GL
{
	std::once_flag GLPlatform::s_InitFlag;
	std::unique_ptr<GLPlatform> GLPlatform::s_Instance;
	GLPlatform::WindowData GLPlatform::s_WindowData = {"Casic GL Window", 1280, 720};
	GLFWwindow* GLPlatform::s_Window = nullptr;

	void GLPlatform::SetWindowData(std::string title, int width, int height)
	{
		s_WindowData.Title = title;
		s_WindowData.Width = width;
		s_WindowData.Height = height;
	}

	void GLPlatform::ErrorCallback(int error_code, const char* description)
	{
		SimpleLogger::GetInstance().Error("glfw error occured. code: {}, desc: {}", error_code, description);
	}

	void GLPlatform::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	}

	void GLPlatform::CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
	}

	void GLPlatform::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
	}

	void GLPlatform::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
	}

	void GLPlatform::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
	}

	GLPlatform::~GLPlatform()
	{

	}

	GLFWwindow* GLPlatform::GetWindow()
	{
		return s_Window;
	}

	GLPlatform::GLPlatform()
	{
	}

	void GLPlatform::InitPlatform()
	{
		glfwSetErrorCallback(ErrorCallback);
		if (glfwInit() == GLFW_FALSE)
		{
			SimpleLogger::GetInstance().Fatal("failed to init glfw...");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// TODO: 
		s_Window = glfwCreateWindow(s_WindowData.Width, s_WindowData.Height, s_WindowData.Title.data(), nullptr, nullptr);
		if(!s_Window) SimpleLogger::GetInstance().Fatal("failed to create glfw window...");

		glfwSetKeyCallback(s_Window, KeyCallback);
		glfwSetCharCallback(s_Window, CharCallback);
		glfwSetMouseButtonCallback(s_Window, MouseButtonCallback);
		glfwSetCursorPosCallback(s_Window, MouseCursorPosCallback);
		glfwSetScrollCallback(s_Window, MouseScrollCallback);

		glfwMakeContextCurrent(s_Window);
		glfwSwapInterval(1);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		SetupViewport(s_WindowData.Width, s_WindowData.Height);
	}

	bool GLPlatform::WindowShouldClose()
	{
		// TODO:
		return glfwWindowShouldClose(s_Window);
	}

	void GLPlatform::ShutdownPlatform()
	{
		glfwDestroyWindow(s_Window);
		glfwTerminate();
	}

	void GLPlatform::SwapBuffers()
	{
		//TODO:
		glfwSwapBuffers(s_Window);
	}

	void GLPlatform::PollInputEvents()
	{
		//TODO:
		glfwPollEvents();
	}

	void GLPlatform::SetupViewport(int width, int height)
	{
		// TODO:
		glViewport(0, 0, width, height);
	}

}
}