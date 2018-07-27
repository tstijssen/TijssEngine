#include "EngineManager.h"
#include <windowsx.h> // for key inputs

TijssEngine::TijssEngine() : m_hwnd(NULL)
{
	
}

TijssEngine::TijssEngine(const TijssEngine& other) : m_hwnd(NULL)
{
}


TijssEngine::~TijssEngine()
{
}

void TijssEngine::RunMessageLoop()
{
	MSG msg;
	bool running, result;

	ZeroMemory(&msg, sizeof(MSG));

	running = false;

	while (!running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			running = true;
		}
		else
		{
			// frame handling
		}

		if (m_Input)
		{
			if (m_Input->IsKeyPressed(0) == true)
			{
				running = true;
			}
		}
	}

	if (m_Graphics)
	{
		m_Graphics->TemporaryMovePosition(D2D_VECTOR_3F{ 1, 0, 0 });
		m_Graphics->OnRender(m_hwnd);
	}
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
	//}
}

bool TijssEngine::Initialize(int& screenWidth, int& screenHeight)
{
	// initialize graphics class
	m_Graphics->Initialize(screenWidth, screenHeight);

	InitializeWindows(screenWidth, screenHeight);

	// initialize input class
	//m_Input = std::make_unique<InputClass>();
	//m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);

	return true;
}

void TijssEngine::InitializeWindows(int& screenWidth, int& screenHeight, char* windowName)
{
	
	HRESULT hr;
	m_hinstance = HINST_THISCOMPONENT;
	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = TijssEngine::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = m_hinstance;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = "D2DDemoApp";

	RegisterClassEx(&wcex);


	// Because the CreateWindow function takes its size in pixels,
	// obtain the system DPI and use it to scale the window size.

	D2D_VECTOR_2F dpi = m_Graphics->GetDPI();
	// The factory returns the current system DPI. This is also the value it will use
	// to create its own windows.


	// Create the window.
	m_hwnd = CreateWindow(
		"D2DDemoApp",
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<UINT>(ceil(screenWidth * dpi.x / 96.f)),
		static_cast<UINT>(ceil(screenHeight * dpi.y / 96.f)),
		NULL,
		NULL,
		m_hinstance,
		this
	);
	hr = m_hwnd ? S_OK : E_FAIL;
	if (SUCCEEDED(hr))
	{
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
		UpdateWindow(m_hwnd);
	}

}

LRESULT CALLBACK TijssEngine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		TijssEngine *pTijssEngine = (TijssEngine *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pTijssEngine)
		);

		result = 1;
	}
	else
	{
		TijssEngine *pTijssEngine = reinterpret_cast<TijssEngine *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pTijssEngine)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pTijssEngine->m_Graphics->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pTijssEngine->m_Graphics->OnRender(pTijssEngine->m_hwnd);
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_KEYDOWN:
				switch (wParam)
				{
				case VK_ESCAPE:
					//PostQuitMessage(0);
					//result = 1;
					break;
				case VK_RIGHT:
					// move right
					if (pTijssEngine->m_Graphics->TemporaryGetX() < 1850)
					{
						pTijssEngine->m_Graphics->TemporaryMovePosition(D2D_VECTOR_3F{ 5, 0, 0 });
						pTijssEngine->m_Graphics->OnRender(pTijssEngine->m_hwnd);
						ValidateRect(hwnd, NULL);
					}
					break;
				case VK_LEFT:
					// move left
					if (pTijssEngine->m_Graphics->TemporaryGetX() > 0)
					{
						pTijssEngine->m_Graphics->TemporaryMovePosition(D2D_VECTOR_3F{ -5, 0, 0 });
						pTijssEngine->m_Graphics->OnRender(pTijssEngine->m_hwnd);
						ValidateRect(hwnd, NULL);
					}
					break;
				case VK_UP:
					// move up
					if (pTijssEngine->m_Graphics->TemporaryGetY() > 0)
					{
						pTijssEngine->m_Graphics->TemporaryMovePosition(D2D_VECTOR_3F{ 0, -5, 0 });
						pTijssEngine->m_Graphics->OnRender(pTijssEngine->m_hwnd);
						ValidateRect(hwnd, NULL);
					}
					break;
				case VK_DOWN:
					// move down
					if (pTijssEngine->m_Graphics->TemporaryGetY() < 945)
					{
						pTijssEngine->m_Graphics->TemporaryMovePosition(D2D_VECTOR_3F{ 0, 5, 0 });
						pTijssEngine->m_Graphics->OnRender(pTijssEngine->m_hwnd);
						ValidateRect(hwnd, NULL);
					}
					break;
				}
				break;

			}
		}


		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}