#include "Input.h"

// Globals

EKeyState  g_aiKeyStates[MaxKeyCodes];
sInt2 g_mousePos;

// Initialisation
void InitInput()
{
	// Initialise input data
	for (int i = 0; i < MaxKeyCodes; ++i)
	{
		g_aiKeyStates[i] = kNotPressed;
	}
	g_mousePos = { 0,0 };
}

// Events

// Event called to indicate that a key has been pressed down
void KeyDownEvent(KeyCodes eKeyCode)
{
	if (g_aiKeyStates[eKeyCode] == kNotPressed)
	{
		g_aiKeyStates[eKeyCode] = kPressed;
	}
	else
	{
		g_aiKeyStates[eKeyCode] = kHeld;
	}
}

// Event called to indicate that a key has been lifted up
void KeyUpEvent(KeyCodes eKeyCode)
{
	g_aiKeyStates[eKeyCode] = kNotPressed;
}

// Returns true when a given key or button is first pressed down.
bool KeyHit(KeyCodes eKeyCode)
{
	if (g_aiKeyStates[eKeyCode] == kPressed)
	{
		g_aiKeyStates[eKeyCode] = kHeld;
		return true;
	}
	return false;
}

// Returns true as long as a given key or button is held down.
bool KeyHeld(KeyCodes eKeyCode)
{
	if (g_aiKeyStates[eKeyCode] == kNotPressed)
	{
		return false;
	}
	g_aiKeyStates[eKeyCode] = kHeld;
	return true;
}

void MouseMoveEvent(int xPos, int yPos)
{
	g_mousePos.x = xPos;
	g_mousePos.y = yPos;
}

sInt2 GetMousePos()
{
	return g_mousePos;
}