#include "GLInput.h"

std::once_flag GLInput::s_InitFlag;
std::unique_ptr<GLInput> GLInput::s_Instance;

GLInput::GLInput()
{
	m_Keyboard.ExitKey = KEY_ESCAPE;
}

bool GLInput::IsKeyPressed(int key)
{
	bool pressed = false;
	auto& input = GLInput::GetInstance();
	if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
	{
		if (input.m_Keyboard.PreviousKeyState[key] == 0 && input.m_Keyboard.CurrentKeyState[key] == 1) pressed = true;
	}
	return pressed;
}

bool GLInput::IsKeyPressedRepeat(int key)
{
	bool repeat = false;
	auto& input = GLInput::GetInstance();
	if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
	{
		if (input.m_Keyboard.KeyRepeatInFrame[key] == 1) repeat = true;
	}

	return repeat;
}

bool GLInput::IsKeyDown(int key)
{
	bool down = false;
	auto& input = GLInput::GetInstance();
	if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
	{
		if (input.m_Keyboard.CurrentKeyState[key] == 1) down = true;
	}

	return down;
}

bool GLInput::IsKeyReleased(int key)
{
	bool released = false;
	auto& input = GLInput::GetInstance();
	if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
	{
		if ((input.m_Keyboard.PreviousKeyState[key] == 1) && (input.m_Keyboard.CurrentKeyState[key] == 0)) released = true;
	}

	return released;
}

bool GLInput::IsKeyUp(int key)
{
	bool up = false;
	auto& input = GLInput::GetInstance();
	if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
	{
		if (input.m_Keyboard.CurrentKeyState[key] == 0) up = true;
	}

	return up;
}

int GLInput::GetKeyPressed()
{
	int value = 0;
	auto& input = GLInput::GetInstance();
	if (input.m_Keyboard.KeyPressedQueueCount > 0)
	{
		value = input.m_Keyboard.KeyPressedQueue[0];

		for (int i = 0; i < input.m_Keyboard.KeyPressedQueueCount - 1; i++)
		{
			input.m_Keyboard.KeyPressedQueue[i] = input.m_Keyboard.KeyPressedQueue[i + 1];
		}

		input.m_Keyboard.KeyPressedQueue[input.m_Keyboard.KeyPressedQueueCount - 1] = 0;
		input.m_Keyboard.KeyPressedQueueCount--;
	}
	return value;
}

int GLInput::GetCharPressed()
{
	int value = 0;
	auto& input = GLInput::GetInstance();
	if (input.m_Keyboard.CharPressedQueueCount > 0)
	{
		// Get character from the queue head
		value = input.m_Keyboard.CharPressedQueue[0];

		// Shift elements 1 step toward the head
		for (int i = 0; i < (input.m_Keyboard.CharPressedQueueCount - 1); i++)
			input.m_Keyboard.CharPressedQueue[i] = input.m_Keyboard.CharPressedQueue[i + 1];

		// Reset last character in the queue
		input.m_Keyboard.CharPressedQueue[input.m_Keyboard.CharPressedQueueCount - 1] = 0;
		input.m_Keyboard.CharPressedQueueCount--;
	}

	return value;
}

void GLInput::SetExitKey(int key)
{
	GLInput::GetInstance().m_Keyboard.ExitKey = key;
}

bool GLInput::IsButtonPressed(int button)
{
	// TODO: 后续可能增加触摸屏的处理
	bool pressed = false;
	auto& input = GLInput::GetInstance();
	if (input.m_Mouse.PreviousButtonState[button] == 0 &&
		input.m_Mouse.CurrentButtonState[button] == 1)
	{
		pressed = true;
	}
	return pressed;
}

bool GLInput::IsButtonDown(int button)
{
	// TODO: 后续可能增加触摸屏的处理
	bool down = false;
	auto& input = GLInput::GetInstance();
	if (input.m_Mouse.CurrentButtonState[button] == 1)
	{
		down = true;
	}
	return down;
}

bool GLInput::IsButtonReleased(int button)
{
	// TODO: 后续可能增加触摸屏的处理
	bool released = false;
	auto& input = GLInput::GetInstance();
	if (input.m_Mouse.PreviousButtonState[button] == 1 &&
		input.m_Mouse.CurrentButtonState[button] == 0)
	{
		released = true;
	}
	return released;
}

bool GLInput::IsButtonUp(int button)
{
	// TODO: 后续可能增加触摸屏的处理
	bool up = false;
	auto& input = GLInput::GetInstance();
	if (input.m_Mouse.CurrentButtonState[button] == 0)
	{
		up = true;
	}
	return up;
}
