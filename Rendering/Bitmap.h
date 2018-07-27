#pragma once
#include <d2d1.h>
#include <Wincodec.h>
#include <string>
#include <memory>


template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

class BitmapClass
{
public:
	BitmapClass() {};
	~BitmapClass() {};

	HRESULT Initialize( );
	HRESULT CreateBitmap(LPCWSTR fileName, ID2D1HwndRenderTarget* renderTarget);

	ID2D1Bitmap* GetBitmap();

	void SetPosition(uint32_t x, uint32_t y, uint32_t z)
	{
		xPos = x;
		yPos = y;
		zPos = z;
	}

	uint32_t GetXPos()
	{
		return xPos;
	}

	uint32_t GetYPos()
	{
		return yPos;
	}

	uint32_t GetZPos()
	{
		return zPos;
	}

private:
	//TODO: convert to unique pointers eventually
	uint32_t xPos;
	uint32_t yPos;
	uint32_t zPos;

	// bitmap file
	ID2D1Bitmap *m_pD2DBitmap;

	IWICBitmapDecoder *m_pDecoder;
	IWICFormatConverter *m_pConvertedSourceBitmap;
	IWICImagingFactory *m_pIWICFactory;

};