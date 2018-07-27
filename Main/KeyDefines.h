#pragma once

#include "windows.h"

	enum EKeyState
	{
		kNotPressed,
		kPressed,
		kHeld
	};

	enum KeyCodes
	{
		// mouse keys
		LeftMouseButton = 0x01,
		RightMouseButton = 0x02,
		MiddleMouseButton = 0x04,

		//keyboard keys
		Key_Back = 0x08,
		Key_Tab = 0x09,
		Key_Return = 0x0D,

		Key_AnyShift = 0x10,
		Key_AnyControl = 0x11,
		Key_AnyAlt = 0x12,

		Key_LeftShift = 0xA0,
		Key_RightShift = 0xA1,
		Key_LeftControl = 0xA2,
		Key_RightControl = 0xA3,
		Key_LeftAlt = 0xA4,
		Key_RightAlt = 0xA5,

		Key_Pause = 0x13,
		Key_CapsLock = 0x14,
		Key_Escape = 0x1B,
		Key_Space = 0x20,
		Key_PageUp = 0x21,
		Key_PageDown = 0x22,
		Key_End = 0x23,
		Key_Home = 0x24,

		Key_Left = 0x25,	// left arrow key
		Key_Up = 0x26,		// up arrow key
		Key_Right = 0x27,	// right arrow key
		Key_Down = 0x28,	// down arrow key

		Key_Insert = 0x2D,
		Key_Delete = 0x2E,

		Key_WindowsR = 0x5B,
		Key_WindowsL = 0x5C,

		Key_Plus = 0xBB,
		Key_Comma = 0xBC,
		Key_Minus = 0xBD,
		Key_Period = 0xBE,

		//keypad
		Key_Numpad0 = 0x60,
		Key_Numpad1 = 0x61,
		Key_Numpad2 = 0x62,
		Key_Numpad3 = 0x63,
		Key_Numpad4 = 0x64,
		Key_Numpad5 = 0x65,
		Key_Numpad6 = 0x66,
		Key_Numpad7 = 0x67,
		Key_Numpad8 = 0x68,
		Key_Numpad9 = 0x69,

		Key_NumLock = 0x90,
		Key_ScrollLock = 0x90,

		//function keys
		Key_F1 = 0x70,
		Key_F2 = 0x71,
		Key_F3 = 0x72,
		Key_F4 = 0x73,
		Key_F5 = 0x74,
		Key_F6 = 0x75,
		Key_F7 = 0x76,
		Key_F8 = 0x77,
		Key_F9 = 0x78,
		Key_F10 = 0x79,
		Key_F11 = 0x7A,
		Key_F12 = 0x7B,
		Key_F13 = 0x7C,
		Key_F14 = 0x7D,
		Key_F15 = 0x7E,
		Key_F16 = 0x7F,
		Key_F17 = 0x80,
		Key_F18 = 0x81,
		Key_F19 = 0x82,
		Key_F20 = 0x83,
		Key_F21 = 0x84,
		Key_F22 = 0x85,
		Key_F23 = 0x86,
		Key_F24 = 0x87,


		//number keys
		Key_0 = 0x30,
		Key_1 = 0x31,
		Key_2 = 0x32,
		Key_3 = 0x33,
		Key_4 = 0x34,
		Key_5 = 0x35,
		Key_6 = 0x36,
		Key_7 = 0x37,
		Key_8 = 0x38,
		Key_9 = 0x39,

		//alphabet keys
		Key_A = 0x41,
		Key_B = 0x42,
		Key_C = 0x43,
		Key_D = 0x44,
		Key_E = 0x45,
		Key_F = 0x46,
		Key_G = 0x47,
		Key_H = 0x48,
		Key_I = 0x49,
		Key_J = 0x4A,
		Key_K = 0x4B,
		Key_L = 0x4C,
		Key_M = 0x4D,
		Key_N = 0x4E,
		Key_O = 0x4F,
		Key_P = 0x50,
		Key_Q = 0x51,
		Key_R = 0x52,
		Key_S = 0x53,
		Key_T = 0x54,
		Key_U = 0x55,
		Key_V = 0x56,
		Key_W = 0x57,
		Key_X = 0x58,
		Key_Y = 0x59,
		Key_Z = 0x5A,

		// number of keys
		MaxKeyCodes = 106
	};