#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <vector>
#include <memory>
#include "Graphics.h"
#include "Input.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class TijssEngine
{
public:
	TijssEngine();
	TijssEngine(const TijssEngine&);
	~TijssEngine();

	bool Initialize(int&, int&);
	// Process and dispatch messages
	void RunMessageLoop();

	void AddLibrary() {};

	//LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
private:
	void InitializeWindows(int&, int&, char* windowName = "TijssEngine");

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	std::unique_ptr<InputClass> m_Input;
	std::unique_ptr<GraphicsClass> m_Graphics = std::make_unique<GraphicsClass>();
};