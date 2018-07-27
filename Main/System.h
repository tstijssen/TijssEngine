#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
// #include "inputclass.h"
// #include "graphics.h"

class SystemManager
{
public:
	SystemManager();
	SystemManager(const SystemManager&);
	~SystemManager();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCSTR m_AppName;
	HINSTANCE m_HInst;
	HWND m_HWND;
	
	
	// InputClass* m_Input;
	// GraphicsClass* m_Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemManager* AppHandle = 0;

// END