#include "GLInput.h"

namespace Casic
{
namespace GL
{
	std::once_flag GLInput::s_InitFlag;
	std::unique_ptr<GLInput> GLInput::s_Instance;
	GLInput::GLInput()
	{

	}

	GLInput::~GLInput()
	{
	}

	bool GLInput::IsKeyPressed(int key)
	{
		bool pressed = false;
		if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
		{
			if (m_Keyboards.PreviousKeyState[key] == KEY_NOT_PRESSED &&
				m_Keyboards.CurrentKeyState[key] == KEY_PRESSED)
				pressed = true;
		}
		return pressed;
	}

	bool GLInput::IsKeyPressedRepeat(int key)
	{
		bool repeat = false;
		if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
		{
			if (m_Keyboards.KeyRepeatInFrame[key] == KEY_REPEAT)
				repeat = true;
		}
		return repeat;
	}

	bool GLInput::IsKeyDown(int key)
	{
		bool down = false;
		if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
		{
			if (m_Keyboards.CurrentKeyState[key] == KEY_PRESSED)
				down = true;
		}
		return down;
	}

	bool GLInput::IsKeyReleased(int key)
	{
		bool released = false;
		if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
		{
			if (m_Keyboards.PreviousKeyState[key] == KEY_PRESSED &&
				m_Keyboards.CurrentKeyState[key] == KEY_NOT_PRESSED)
				released = true;
		}
		return released;
	}

	bool GLInput::IsKeyUp(int key)
	{
		bool up = false;
		if ((key > 0) && (key < MAX_KEYBOARD_KEYS))
		{
			if (m_Keyboards.CurrentKeyState[key] == KEY_NOT_PRESSED)
				up = true;
		}
		return up;
	}

	int GLInput::GetKeyPressed()
	{
		int key = -1;
		if (m_Keyboards.KeyPressedQueueCount > 0)
		{
			key = m_Keyboards.KeyPressedQueue[0];
			for (int i = 0; i < m_Keyboards.KeyPressedQueueCount - 1; i++)
			{
				m_Keyboards.KeyPressedQueue[i] = m_Keyboards.KeyPressedQueue[i + 1];
			}
			m_Keyboards.KeyPressedQueue[m_Keyboards.KeyPressedQueueCount - 1] = 0;
			m_Keyboards.KeyPressedQueueCount--;
		}
		return key;
	}

	int GLInput::GetCharPressed()
	{
		int c = -1;
		if (m_Keyboards.CharPressedQueueCount > 0)
		{
			c = m_Keyboards.CharPressedQueue[0];
			for (int i = 0; i < m_Keyboards.CharPressedQueueCount - 1; i++)
			{
				m_Keyboards.CharPressedQueue[i] = m_Keyboards.CharPressedQueue[i + 1];
			}
			m_Keyboards.CharPressedQueue[m_Keyboards.CharPressedQueueCount - 1] = 0;
			m_Keyboards.CharPressedQueueCount--;
		}
		return c;
	}

	void GLInput::SetExitKey(int key)
	{
		m_Keyboards.ExitKey = key;
	}

	void GLInput::SetMouseOffset(int offsetX, int offsetY)
	{
		m_Mouse.Offset = { (float)offsetX, (float)offsetY };
	}

	void GLInput::SetMouseScale(float scaleX, float scaleY)
	{
		m_Mouse.Scale = { scaleX, scaleY };
	}

	Math::Vector2 GLInput::GetMousePosition()
	{
		// NOTE: The mouse position within the rendering area, not the entire window.
		Math::Vector2 pos;
		pos.x = (m_Mouse.CurrentPosition.x + m_Mouse.Offset.x) * m_Mouse.Scale.x;
		pos.x = (m_Mouse.CurrentPosition.y + m_Mouse.Offset.y) * m_Mouse.Scale.y;
		return pos;
	}

	int GLInput::GetMouseX()
	{
		// NOTE: The mouse position within the rendering area, not the entire window.
		int x = (int)((m_Mouse.CurrentPosition.x + m_Mouse.Offset.x) * m_Mouse.Scale.x);
		return x;
	}

	int GLInput::GetMouseY()
	{
		// NOTE: The mouse position within the rendering area, not the entire window.
		int y = (int)((m_Mouse.CurrentPosition.y + m_Mouse.Offset.y) * m_Mouse.Scale.y);
		return y;
	}

	Math::Vector2 GLInput::GetMouseDelta()
	{
		Math::Vector2 delta;
		delta.x = m_Mouse.CurrentPosition.x - m_Mouse.PreviousPosition.x;
		delta.y = m_Mouse.CurrentPosition.y - m_Mouse.PreviousPosition.y;
		return delta;
	}

	bool GLInput::IsMouseButtonPressed(int button)
	{
		bool pressed = false;
		if (m_Mouse.PreviousButtonState[button] == BUTTON_NOT_PRESSED && m_Mouse.CurrentButtonState[button] == BUTTON_PRESSED)
		{
			pressed = true;
		}
		return pressed;
	}

	bool GLInput::IsMouseButtonDown(int button)
	{
		bool down = false;
		if (m_Mouse.CurrentButtonState[button] == BUTTON_PRESSED)
		{
			down = true;
		}
		return down;
	}

	bool GLInput::IsMouseButtonReleased(int button)
	{
		bool released = false;
		if (m_Mouse.PreviousButtonState[button] == BUTTON_PRESSED && m_Mouse.CurrentButtonState[button] == BUTTON_NOT_PRESSED)
		{
			released = true;
		}
		return released;
	}

	bool GLInput::IsMouseButtonUp(int button)
	{
		bool up = false;
		if (m_Mouse.CurrentButtonState[button] == BUTTON_NOT_PRESSED)
		{
			up = true;
		}
		return up;
	}

	float GLInput::GetMouseWheel()
	{
		return m_Mouse.CurrentWheelMove.x > m_Mouse.CurrentWheelMove.y ? 
			m_Mouse.CurrentWheelMove.x :
			m_Mouse.CurrentWheelMove.y;
	}

	Math::Vector2 GLInput::GetMouseWheelV()
	{
		Math::Vector2 result{ m_Mouse.CurrentWheelMove.x, m_Mouse.CurrentWheelMove.y };
		return result;
	}

}
}