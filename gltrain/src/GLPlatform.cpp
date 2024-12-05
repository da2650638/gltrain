#include "GLPlatform.h"
#include "SimpleLogger.h"
#include "GLInput.h"

namespace Casic
{
namespace GL
{
	std::once_flag GLPlatform::s_InitFlag;
	std::unique_ptr<GLPlatform> GLPlatform::s_Instance;

	void GLPlatform::SetWindowData(std::string title, int width, int height)
	{
		m_WindowData.Title = title;
		m_WindowData.Width = width;
		m_WindowData.Height = height;
	}

	GLPlatform::Time& GLPlatform::TimeData()
	{
		return m_TimeData;
	}

	float GLPlatform::GetTime()
	{
		return glfwGetTime();
	}

	void GLPlatform::ErrorCallback(int error_code, const char* description)
	{
		SimpleLogger::GetInstance().Error("glfw error occured. code: {}, desc: {}", error_code, description);
	}

	void GLPlatform::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto& platform = GLPlatform::GetInstance();
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
			glfwSetWindowShouldClose(platform.m_Window, GLFW_TRUE);
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

	GLFWwindow* GLPlatform::Window()
	{
		return m_Window;
	}

	GLPlatform::WindowData& GLPlatform::GetWindowData()
	{
		return m_WindowData;
	}

	GLPlatform::GLPlatform(WindowData wd)
	{
		m_Window = nullptr;
		m_WindowData.Title = wd.Title;
		m_WindowData.Width = wd.Width;
		m_WindowData.Height = wd.Height;
		m_TimeData = { 0 };
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
		m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.data(), nullptr, nullptr);
		if(!m_Window) SimpleLogger::GetInstance().Fatal("failed to create glfw window...");

		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetCharCallback(m_Window, CharCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetCursorPosCallback(m_Window, MouseCursorPosCallback);
		glfwSetScrollCallback(m_Window, MouseScrollCallback);

		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(0);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}

	bool GLPlatform::WindowShouldClose()
	{
		// TODO:
		return glfwWindowShouldClose(m_Window);
	}

	void GLPlatform::ShutdownPlatform()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void GLPlatform::SwapBuffers()
	{
		//TODO:
		glfwSwapBuffers(m_Window);
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
}
}