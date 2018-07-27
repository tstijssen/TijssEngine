#pragma once
#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <dwrite.h>
#include "TypeDefines.h"
#include "Graphics.h"

namespace Tijss
{
	class cText
	{
		IDWriteFactory* m_TextFactory;
		IDWriteTextFormat* m_TextFormat;
		IDWriteTextLayout* m_TextLayout;
		D2D1_RECT_F layoutRect;
		std::wstring m_WszText_;
		std::wstring m_FontName;

		UINT32 m_cTextLength;

		ID2D1DeviceContext5* m_D2DContext;
		ID2D1SolidColorBrush* m_Brush;
		RECT rc;
	public:
		cText(Graphics* gfx, std::wstring fontName, float fontSize, std::wstring text = L"");
		~cText();

		void Draw();
		void Draw(std::wstring text, int x, int y);
		void Draw(std::wstring text, sInt2 xy);
		void Draw(int x, int y);
		void Draw(sInt2 xy);
		void Draw(sFloat2 xy);

		void SetColour(sColour colour);
		void SetPosition(int x, int y);
		void SetPosition(sInt2 xy);
		void SetPosition(sFloat2 xy);

		void SetText(std::wstring text);
	};

}

