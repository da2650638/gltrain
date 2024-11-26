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
		GLMath::Vector2 Offset{ 0,0 };
		GLMath::Vector2 Scale{ 0,0 };
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