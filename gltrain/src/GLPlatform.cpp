#include "GLPlatform.h"
#include "SimpleLogger.h"
#include "GLInput.h"

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
		auto& input = GLInput::GetInstance();
		if (action == GLFW_PRESS)
		{
			input.m_Keyboards.CurrentKeyState[key] = KEY_PRESSED;
		}
		else if (action == GLFW_RELEASE)
		{
			input.m_Keyboards.CurrentKeyState[key] = KEY_NOT_PRESSED;
		}
		else if (action == GLFW_REPEAT)
		{
			input.m_Keyboards.KeyRepeatInFrame[key] = KEY_REPEAT;
		}

		if (key == GLFW_KEY_CAPS_LOCK && ((mods & GLFW_MOD_CAPS_LOCK) != 0))
		{
			input.m_Keyboards.CurrentKeyState[key] == KEY_PRESSED;
		}
		if (key == GLFW_KEY_NUM_LOCK && ((mods & GLFW_MOD_NUM_LOCK) != 0))
		{
			input.m_Keyboards.CurrentKeyState[key] == KEY_PRESSED;
		}

		if (input.m_Keyboards.KeyPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
		{
			input.m_Keyboards.KeyPressedQueue[input.m_Keyboards.KeyPressedQueueCount] = key;
			input.m_Keyboards.KeyPressedQueueCount++;
		}

		if(input.m_Keyboards.ExitKey == key && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(s_Window, GLFW_TRUE);
		}
	}

	void GLPlatform::CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		auto& input = GLInput::GetInstance();
		if (input.m_Keyboards.CharPressedQueueCount < MAX_CHAR_PRESSED_QUEUE)
		{
			input.m_Keyboards.CharPressedQueue[input.m_Keyboards.CharPressedQueueCount] = codepoint;
			input.m_Keyboards.CharPressedQueueCount++;
		}
	}

	void GLPlatform::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto& input = GLInput::GetInstance();
		input.m_Mouse.CurrentButtonState[button] = action;
	}

	void GLPlatform::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		auto& input = GLInput::GetInstance();
		input.m_Mouse.CurrentPosition = { (float)xpos, (float)ypos };
	}

	void GLPlatform::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto& input = GLInput::GetInstance();
		input.m_Mouse.CurrentWheelMove = { (float)xoffset, (float)yoffset};
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

		//----------------------------------------------------
		// Init Input Module
		//----------------------------------------------------
		auto& input = GLInput::GetInstance();
		for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
		{
			input.m_Keyboards.PreviousKeyState[i] = input.m_Keyboards.CurrentKeyState[i];
			input.m_Keyboards.KeyRepeatInFrame[i] = KEY_NOT_REPEAT;
		}
		input.m_Keyboards.KeyPressedQueueCount = 0;
		input.m_Keyboards.CharPressedQueueCount = 0;

		input.m_Mouse.PreviousPosition = input.m_Mouse.CurrentPosition;
		for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
		{
			input.m_Mouse.PreviousButtonState[i] = input.m_Mouse.CurrentButtonState[i];
		}
		input.m_Mouse.PreviousWheelMove = input.m_Mouse.CurrentWheelMove;
		input.m_Mouse.CurrentWheelMove = { 0,0 };

		glfwPollEvents();
	}

	void GLPlatform::SetupViewport(int width, int height)
	{
		// TODO:
		glViewport(0, 0, width, height);
	}

}
}