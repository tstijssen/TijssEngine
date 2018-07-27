#include "Bitmap.h"

HRESULT BitmapClass::Initialize()
{
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	return hr;
}

HRESULT BitmapClass::CreateBitmap(LPCWSTR fileName, ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr;

	hr = m_pIWICFactory->CreateDecoderFromFilename(
		fileName,		// image file name
		NULL,
		GENERIC_READ,					//access level to file
		WICDecodeMetadataCacheOnDemand,	// cache metadeta
		&m_pDecoder
	);
	// get first frame of image from decoder
	IWICBitmapFrameDecode *pFrame = NULL;
	if (SUCCEEDED(hr))
	{
		hr = m_pDecoder->GetFrame(0, &pFrame);
	}

	// Format convert the frame to 32bppPBGRA
	if (SUCCEEDED(hr))
	{
		SafeRelease(&m_pConvertedSourceBitmap);
		hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pConvertedSourceBitmap->Initialize(
			pFrame,                          // Input bitmap to convert
			GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
			WICBitmapDitherTypeNone,         // Specified dither pattern
			NULL,                            // Specify a particular palette 
			0.0f,                             // Alpha threshold
			WICBitmapPaletteTypeCustom       // Palette translation type
		);
	}

	if (m_pConvertedSourceBitmap && !m_pD2DBitmap)
	{
		renderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, NULL, &m_pD2DBitmap);
	}
	return hr;
}

ID2D1Bitmap* BitmapClass::GetBitmap()
{
	return m_pD2DBitmap;
}