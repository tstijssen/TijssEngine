#include "System.h"

SystemManager::SystemManager()
{

}

SystemManager::SystemManager(const SystemManager& other)
{

}

SystemManager::~SystemManager()
{

}

bool SystemManager::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);


	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	//m_Graphics = new GraphicsClass;
	//if (!m_Graphics)
	//{
	//	return false;
	//}

	//// Initialize the graphics object.
	//result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}

void SystemManager::Shutdown()
{
	// Release the graphics object.
	//if (m_Graphics)
	//{
	//	m_Graphics->Shutdown();
	//	delete m_Graphics;
	//	m_Graphics = 0;
	//}

	// Shutdown the window.
	ShutdownWindows();

	return;

}

void SystemManager::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}
	return;
}

bool SystemManager::Frame()
{
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (KeyHit(Key_Escape))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	//result = m_Graphics->Frame();
	//if (!result)
	//{
	//	return false;
	//}

	return true;
}

void SystemManager::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	AppHandle = this;

	// Get the instance of this application.
	m_HInst = GetModuleHandle(NULL);

	// Give the application a name.
	m_AppName = "Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_HInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_AppName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (CDS_FULLSCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_HWND = CreateWindowEx(WS_EX_APPWINDOW, m_AppName, m_AppName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_HInst, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_HWND, SW_SHOW);
	SetForegroundWindow(m_HWND);
	SetFocus(m_HWND);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemManager::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (CDS_FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_HWND);
	m_HWND = NULL;

	// Remove the application instance.
	UnregisterClass(m_AppName, m_HInst);
	m_HInst = NULL;

	// Release the pointer to this class.
	AppHandle = NULL;

	return;
}

LRESULT CALLBACK SystemManager::MessageHandler(
	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
)
{
	// read keys and buttons
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		KeyCodes eKeyCode = static_cast<KeyCodes>(wParam);
		KeyDownEvent(eKeyCode);
		break;
	}

	case WM_KEYUP:
	{
		KeyCodes eKeyCode = static_cast<KeyCodes>(wParam);
		KeyUpEvent(eKeyCode);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		KeyDownEvent(LeftMouseButton);
		break;
	}
	case WM_LBUTTONUP:
	{
		KeyUpEvent(LeftMouseButton);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		KeyDownEvent(RightMouseButton);
		break;
	}
	case WM_RBUTTONUP:
	{
		KeyUpEvent(RightMouseButton);

		break;
	}
	case WM_MBUTTONDOWN:
	{
		KeyDownEvent(MiddleMouseButton);
		break;
	}
	case WM_MBUTTONUP:
	{
		KeyUpEvent(MiddleMouseButton);
		break;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return AppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}