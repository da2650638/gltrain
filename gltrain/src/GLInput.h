#pragma once

#include <mutex>
#include <memory>

#include "GLGlobal.h"

namespace Casic
{
namespace GL
{
	class GLInput
	{
	public:
		~GLInput();
		static GLInput& GetInstance()
		{
			std::call_once(s_InitFlag, []() {
				s_Instance.reset(new GLInput());
			});
			return *s_Instance;
		}

		//----------------------------------------------------
		// Key/Char Events
		//----------------------------------------------------
		bool IsKeyPressed(int key);
		bool IsKeyPressedRepeat(int key);
		bool IsKeyDown(int key);
		bool IsKeyReleased(int key);
		bool IsKeyUp(int key);
		int GetKeyPressed();
		int GetCharPressed();
		void SetExitKey(int key);
		//----------------------------------------------------
		// MouseButton/MouseWheel Events
		// TODO: 现在只能处理鼠标，后续可能还要增加触摸屏的处理
		//----------------------------------------------------
		void SetMouseOffset(int offsetX, int offsetY);
		void SetMouseScale(float scaleX, float scaleY);
		Vector2 GetMousePosition();
		int GetMouseX();
		int GetMouseY();
		Vector2 GetMouseDelta();
		bool IsMouseButtonPressed(int button);
		bool IsMouseButtonDown(int button);
		bool IsMouseButtonReleased(int button);
		bool IsMouseButtonUp(int button);
		float GetMouseWheel();
		Vector2 GetMouseWheelV();

	private:
		GLInput();

	public:
		typedef struct Keyboards {
			int ExitKey = KEY_ESCAPE;
			char PreviousKeyState[MAX_KEYBOARD_KEYS]{ 0 };
			char CurrentKeyState[MAX_KEYBOARD_KEYS]{ 0 };

			char KeyRepeatInFrame[MAX_KEYBOARD_KEYS]{ 0 };

			int KeyPressedQueue[MAX_KEY_PRESSED_QUEUE]{ 0 };
			int KeyPressedQueueCount = 0;

			int CharPressedQueue[MAX_CHAR_PRESSED_QUEUE]{ 0 };
			int CharPressedQueueCount = 0;
		}Keyboards;
		typedef struct Mouse {
			Vector2 Offset{ 0,0 };
			Vector2 Scale{ 1.0, 1.0 };
			Vector2 PreviousPosition;
			Vector2 CurrentPosition;

			int Cursor;
			bool CursorHidden;
			bool CursorOnScreen;

			char PreviousButtonState[MAX_MOUSE_BUTTONS]{ 0 };
			char CurrentButtonState[MAX_MOUSE_BUTTONS]{ 0 };
			Vector2 PreviousWheelMove;
			Vector2 CurrentWheelMove;
		}Mouse;
		Keyboards m_Keyboards;
		Mouse m_Mouse;
		
	private:
		static std::once_flag s_InitFlag;
		static std::unique_ptr<GLInput> s_Instance;
	};
}
}