#include "Graphics.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D2d1.lib")

using namespace DirectX;
namespace Tijss
{
	Graphics::Graphics()
	{
		m_D3DDevice = NULL;
		m_D3DContext = NULL;
		m_D2DFactory = NULL;
		m_D2DDevice = NULL;
		m_D2DContext = NULL;
		m_DXGISwapChain = NULL;
		m_D2DBackBuffer = NULL;
	}

	Graphics::~Graphics()
	{
		//if(m_Factory) m_Factory->Release();
		//if(m_RenderTarget) m_RenderTarget->Release();
		//if (m_Brush) m_Brush->Release();

		if (m_Camera) delete m_Camera;
		if (m_D3DDevice) m_D3DDevice->Release();
		if (m_D3DContext) m_D3DContext->Release();
		if (m_D2DFactory) m_D2DFactory->Release();
		if (m_D2DDevice) m_D2DDevice->Release();
		if (m_D2DContext) m_D2DContext->Release();
		if (m_DXGISwapChain) m_DXGISwapChain->Release();
		if (m_D2DBackBuffer) m_D2DBackBuffer->Release();
	}


	bool Graphics::Init(HWND windowHandle)
	{
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

		HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void **>(&m_D2DFactory));
		if (result != S_OK) return false;

		RECT rect;
		GetClientRect(windowHandle, &rect);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		// Create Direct3D device and context
		ID3D11Device *device;
		ID3D11DeviceContext *context;
		D3D_FEATURE_LEVEL returnedFeatureLevel;

		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
			&device, &returnedFeatureLevel, &context);

		device->QueryInterface(__uuidof(ID3D11Device1), (void **)&m_D3DDevice);
		context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&m_D3DContext);

		IDXGIDevice *dxgiDevice;

		m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);

		m_D2DFactory->CreateDevice(dxgiDevice, &m_D2DDevice);

		m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_D2DContext);

		// Get the GPU we are using
		IDXGIAdapter *dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		// Get the DXGI factory instance
		IDXGIFactory2 *dxgiFactory;
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		// Describe Windows 7-compatible Windowed swap chain
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

		swapChainDesc.Width = 0;
		swapChainDesc.Height = 0;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		// Create DXGI swap chain targeting a window handle (the only Windows 7-compatible option)
		dxgiFactory->CreateSwapChainForHwnd(m_D3DDevice, windowHandle, &swapChainDesc, nullptr, nullptr, &m_DXGISwapChain);

		// Get the back buffer as an IDXGISurface (Direct2D doesn't accept an ID3D11Texture2D directly as a render target)
		IDXGISurface *dxgiBackBuffer;
		m_DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

		// Get screen DPI
		FLOAT dpiX, dpiY;
		m_D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

		// Create a Direct2D surface (bitmap) linked to the Direct3D texture back buffer via the DXGI back buffer
		D2D1_BITMAP_PROPERTIES1 bitmapProperties =
			D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);

		m_D2DContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &m_D2DBackBuffer);

		// Set surface as render target in Direct2D device context
		m_D2DContext->SetTarget(m_D2DBackBuffer);

		m_WorldMatrix = DirectX::XMMatrixIdentity();
		m_OrthoMatrix = DirectX::XMMatrixOrthographicLH(m_D2DContext->GetPixelSize().width, m_D2DContext->GetPixelSize().height, 0.1f, 1000.0f);

		m_Camera = new Camera;

		m_Camera->SetPosition(0, 0, 0.0f);

		m_Window = windowHandle;

		//m_Text = new cText(m_D2DFactory, m_D2DContext, windowHandle);

		if (dxgiBackBuffer) dxgiBackBuffer->Release();
		if (dxgiFactory) dxgiFactory->Release();
		if (dxgiAdapter) dxgiAdapter->Release();
		if (dxgiDevice) dxgiDevice->Release();
		if (context) context->Release();
		if (device) device->Release();

		return true;
	}

	void Graphics::ClearScreen(float r, float g, float b)
	{
		m_D2DContext->Clear(D2D1::ColorF(r, g, b));
	}

	void Graphics::BeginDraw()
	{
		XMMATRIX worldMatrix, viewMatrix, projMatrix;
		HRESULT result;

		m_D2DContext->BeginDraw();
	}

	void Graphics::EndDraw()
	{
		m_D2DContext->EndDraw();
		DXGI_PRESENT_PARAMETERS parameters = { 0 };
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;

		HRESULT hr = m_DXGISwapChain->Present1(1, 0, &parameters);
	}

}