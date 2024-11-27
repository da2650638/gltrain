#include "GLGlobalState.h"

#include "GLRenderBatch.h"

std::once_flag GLGlobalState::m_InitFlag;
std::unique_ptr<GLGlobalState> GLGlobalState::m_Instance;
GLGlobalState::Time GLGlobalState::s_Time{ 0 };
GLGlobalState::WindowData GLGlobalState::s_WindowData;
GLFWwindow* GLGlobalState::s_Window = nullptr;

void GLGlobalState::errorCallback(int error_code, const char* description)
{
	GLSimpleLogger::GetInstance().Error("Error: [Code: {}], [Description:{}]\n", error_code, description);
}

void GLGlobalState::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& inputModule = GLInput::GetInstance();
	if (action == GLFW_RELEASE)
	{
		inputModule.m_Keyboard.CurrentKeyState[key] = 0;
	}
	else if (action == GLFW_PRESS) inputModule.m_Keyboard.CurrentKeyState[key] = 1;
	else if (action == GLFW_REPEAT) inputModule.m_Keyboard.KeyRepeatInFrame[key] = 1;

	if((key == GLFW_KEY_CAPS_LOCK) && ((mods & GLFW_MOD_CAPS_LOCK) > 0)) inputModule.m_Keyboard.CurrentKeyState[key] = 1;
	if((key == GLFW_KEY_NUM_LOCK) && ((mods & GLFW_MOD_NUM_LOCK) > 0)) inputModule.m_Keyboard.CurrentKeyState[key] = 1;

	if (inputModule.m_Keyboard.KeyPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
	{
		inputModule.m_Keyboard.KeyPressedQueue[inputModule.m_Keyboard.KeyPressedQueueCount] = key;
		inputModule.m_Keyboard.KeyPressedQueueCount++;
	}

	if (key == inputModule.m_Keyboard.ExitKey && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void GLGlobalState::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	auto& inputModule = GLInput::GetInstance();
	if (inputModule.m_Keyboard.CharPressedQueueCount < MAX_CHAR_PRESSED_QUEUE)
	{
		inputModule.m_Keyboard.CharPressedQueue[inputModule.m_Keyboard.CharPressedQueueCount] = codepoint;
		inputModule.m_Keyboard.CharPressedQueueCount++;
	}
}

void GLGlobalState::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto& input = GLInput::GetInstance();
	// TODO: 暂时只能处理鼠标，后续可能会增加触摸屏等
	input.m_Mouse.CurrentButtonState[button] = action;
}

void GLGlobalState::mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	auto& input = GLInput::GetInstance();
	// TODO: 暂时只能处理鼠标，后续可能会增加触摸屏和手势等等
	input.m_Mouse.CurrentPosition = { (float)xpos, (float)ypos };
}

void GLGlobalState::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto& input = GLInput::GetInstance();
	input.m_Mouse.CurrentWheelMove = { (float)xoffset, (float)yoffset };
}

void GLGlobalState::InitPlatform(const WindowData& wd)
{
	s_WindowData = wd;
	
	glfwSetErrorCallback(GLGlobalState::errorCallback);

	if (!glfwInit())
	{
		GLSimpleLogger::GetInstance().Fatal("Init glfw failed.");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	s_Window = glfwCreateWindow(wd.Width, wd.Height, wd.Title.data(), nullptr, nullptr);
	if (!s_Window) return GLSimpleLogger::GetInstance().Fatal("Create glfw window failed.");;

	glfwSetKeyCallback(s_Window, keyCallback);
	glfwSetCharCallback(s_Window, charCallback);
	glfwSetMouseButtonCallback(s_Window, mouseButtonCallback);
	glfwSetCursorPosCallback(s_Window, mouseCursorPosCallback);
	glfwSetScrollCallback(s_Window, mouseScrollCallback);

	glfwMakeContextCurrent(s_Window);
	glfwSwapInterval(1);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// OpenGL global option
	// TODO: 这个函数应该拿出去放到另一个函数里面，作为OpenGL的初始化的一部分
	glViewport(0, 0, 1280, 720);
}

bool GLGlobalState::WindowShouldClose()
{
	return glfwWindowShouldClose(s_Window);
}

void GLGlobalState::ShutdownPlatform()
{
	glfwDestroyWindow(s_Window);
	glfwTerminate();
}

GLGlobalState::ShaderInfo GLGlobalState::LoadShaderInfos(const std::string& filename, const std::string& shaderName)
{
	YAML::Node config = YAML::LoadFile(filename);

	ShaderInfo shaderInfo;
	shaderInfo.LocationDatas = std::unordered_map<std::string, LocationData>{};
	for (const auto& shader : config["shaders"])
	{
		if (shader["name"].as<std::string>() == shaderName)
		{
			if (shader["attributes"])
			{
				for (const auto& attribute : shader["attributes"])
				{
					LocationData locData;
					locData.Name = attribute.second["name"].as<std::string>();
					locData.DefaultLoc = attribute.second["defaultLoc"].as<unsigned int>();
					locData.IndexInLocArray = attribute.second["indexInLocArray"].as<unsigned int>();
					locData.Type = attribute.second["type"].as < std::string>();
					locData.LocationType = "attribute";
					shaderInfo.LocationDatas[locData.Name] = locData;
				}
			}
			if (shader["uniforms"])
			{
				for (const auto& uniform : shader["uniforms"])
				{
					LocationData locData;
					locData.Name = uniform.second["name"].as<std::string>();
					locData.DefaultLoc = 0;  // Uniform需要实时从shader获取
					locData.IndexInLocArray = uniform.second["indexInLocArray"].as<unsigned int>();
					locData.Type = uniform.second["type"].as<std::string>();
					locData.LocationType = "uniform";
					shaderInfo.LocationDatas[locData.Name] = locData;
				}
			}
		}
	}

	//GLSimpleLogger& logger = GLSimpleLogger::GetInstance();
	//logger.Debug("ShaderInfo: ", shaderInfo.VertexShaderFile);
	//logger.Debug("Vertex Shader File: {}", shaderInfo.VertexShaderFile);
	//logger.Debug("Fragment Shader File: {}", shaderInfo.FragmentShaderFile);

	//for (const auto& [key, locData] : shaderInfo.LocationDatas) {
	//	logger.Debug("Location Data - Name: {}, DefaultLoc: {}, IndexInLocArray: {}, Type: {}",
	//		locData.Name, locData.DefaultLoc, locData.IndexInLocArray, locData.Type);
	//}
	return shaderInfo;
}

void GLGlobalState::PollInputEvent()
{
	// TODO: WIP
	auto& input = GLInput::GetInstance();

	input.m_Keyboard.KeyPressedQueueCount = 0;
	input.m_Keyboard.CharPressedQueueCount = 0;

	for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
	{
		input.m_Keyboard.PreviousKeyState[i] = input.m_Keyboard.CurrentKeyState[i];
		input.m_Keyboard.KeyRepeatInFrame[i] = 0;
	}

	input.m_Mouse.PreviousPosition = input.m_Mouse.CurrentPosition;

	input.m_Mouse.PreviousWheelMove = input.m_Mouse.CurrentWheelMove;
	input.m_Mouse.CurrentWheelMove = { 0,0 };

	for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		input.m_Mouse.PreviousButtonState[i] = input.m_Mouse.CurrentButtonState[i];
	}

	// NOTE: Before this, all event-related data structures must be set up
	glfwPollEvents();
}

void GLGlobalState::BeginDrawing()
{
	s_Time.Current = GetTime();
	s_Time.Update = s_Time.Current - s_Time.Previous;
	s_Time.Previous = s_Time.Current;

	// TODO: 还有一些其他的数据可以设置。
}

void GLGlobalState::EndDrawing(GLRenderBatch* activeBatch)
{
	activeBatch->DrawRenderBatch();

	// TODO: This functionality should be improved later.
	glfwSwapBuffers(s_Window);

	s_Time.Current = GetTime();
	s_Time.Draw = s_Time.Current - s_Time.Previous;
	s_Time.Previous = s_Time.Current;
	s_Time.Frame = s_Time.Draw + s_Time.Update;

	// TODO: To control the frame rate, a delay may be needed. Implementation will be added later.

	// TODO: PollInputEvent(WIP)
	PollInputEvent();
}
