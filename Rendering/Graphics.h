#pragma once

#include <Windows.h>
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Camera.h"

//TODO: ERROR CHECKING ON INIT

namespace Tijss
{
	class Graphics
	{
		ID3D11Device1* m_D3DDevice;
		ID3D11DeviceContext1* m_D3DContext;
		ID2D1Factory6* m_D2DFactory;
		ID2D1Device5* m_D2DDevice;
		ID2D1DeviceContext5* m_D2DContext;
		IDXGISwapChain1* m_DXGISwapChain;
		ID2D1Bitmap1* m_D2DBackBuffer;

		DirectX::XMMATRIX m_WorldMatrix;
		DirectX::XMMATRIX m_OrthoMatrix;
		Camera* m_Camera;
		HWND m_Window;
	public:
		Graphics();
		~Graphics();

		bool Init(HWND windowHandle);

		ID2D1DeviceContext5* GetContext()
		{
			return m_D2DContext;
		}

		ID2D1Factory6* GetFactory()
		{
			return m_D2DFactory;
		}

		ID2D1Device5* GetDevice()
		{
			return m_D2DDevice;
		}

		HWND GetWindowHWND()
		{
			return m_Window;
		}

		void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
		{
			worldMatrix = m_WorldMatrix;
			return;
		}

		Camera* GetCamera()
		{
			return m_Camera;
		}

		void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
		{
			orthoMatrix = m_OrthoMatrix;
			return;
		}

		void BeginDraw();
		void EndDraw();
		void ClearScreen(float r, float g, float b);
		//void DrawCircle(float x, float y, float radius, float r, float g, float b, float a);
	};
}
