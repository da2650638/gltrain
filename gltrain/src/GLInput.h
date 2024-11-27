#pragma once
#include <mutex>
#include <memory>

#include "GLGlobal.h"
#include "GLMath.h"

class GLInput {
public:
	~GLInput() = default;

	static GLInput& GetInstance()
	{
		std::call_once(s_InitFlag, []() {
			s_Instance.reset(new GLInput());
		});
		return *s_Instance;
	}

	// Key-input functions
	bool IsKeyPressed(int key);
	bool IsKeyPressedRepeat(int key);
	bool IsKeyDown(int key);
	bool IsKeyReleased(int key);
	bool IsKeyUp(int key);
	int GetKeyPressed();
	int GetCharPressed();
	void SetExitKey(int key);
	// Mouse button input functions
	bool IsButtonPressed(int button);
	bool IsButtonDown(int button);
	bool IsButtonReleased(int button);
	bool IsButtonUp(int button);
	void SetMouseOffset(int offsetX, int offsetY);
	void SetMouseScale(float scaleX, float scaleY);
	GLMath::Vector2 GetMousePosition();
	int GetMouseX();
	int GetMouseY();
	GLMath::Vector2 GetMouseDelta();
	float GetMouseWheelMove();
	GLMath::Vector2 GetMouseWheelMoveV();

public:
	typedef struct Keyboard {
		int ExitKey = 0;
		char CurrentKeyState[MAX_KEYBOARD_KEYS]{ 0 };
		char PreviousKeyState[MAX_KEYBOARD_KEYS]{ 0 };

		char KeyRepeatInFrame[MAX_KEYBOARD_KEYS]{ 0 };

		int KeyPressedQueue[MAX_KEY_PRESSED_QUEUE]{ 0 };
		int KeyPressedQueueCount = 0;

		int CharPressedQueue[MAX_CHAR_PRESSED_QUEUE]{ 0 };
		int CharPressedQueueCount = 0;
	}Keyboard;
	typedef struct Mouse{
		// 当渲染目标的左上角（或者其他原点）不在窗口的左上角（或者其他原点）时
		// 需要使用Offset来校正鼠标位置，使之与渲染区域对齐
		GLMath::Vector2 Offset{ 0,0 };
		// 当渲染区域的分辨率和窗口的分辨率不一致是需要使用Scale进行校正
		// 正确匹配渲染区域的分辨率
		GLMath::Vector2 Scale{ 1.0, 1.0 };
		// 这里的位置表示鼠标在渲染区域内的相对位置
		GLMath::Vector2 CurrentPosition{ 0,0 };
		GLMath::Vector2 PreviousPosition{ 0,0 };

		int Cursor = 0;
		bool CursorHidden = false;
		bool CursorOnScreen = true;

		char CurrentButtonState[MAX_MOUSE_BUTTONS]{ 0 };
		char PreviousButtonState[MAX_MOUSE_BUTTONS]{ 0 };
		GLMath::Vector2 CurrentWheelMove{ 0,0 };
		GLMath::Vector2 PreviousWheelMove{ 0,0 };
	}Mouse;
	Keyboard m_Keyboard;
	Mouse m_Mouse;

private:
	GLInput();
	GLInput(const GLInput&) = delete;
	GLInput& operator=(const GLInput&) = delete;

	static std::once_flag s_InitFlag;
	static std::unique_ptr<GLInput> s_Instance;
};