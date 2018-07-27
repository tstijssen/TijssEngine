#include "TextRender.h"
#pragma comment(lib,"Dwrite.lib")

namespace Tijss
{
	cText::cText(Graphics* gfx, std::wstring fontName, float fontSize, std::wstring text)
	{
		HRESULT hr;
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_TextFactory)
		);

		m_D2DContext = gfx->GetContext();

		m_WszText_ = text;
		m_FontName = fontName;
		m_cTextLength = (UINT32)wcslen(m_WszText_.c_str());

		hr = m_TextFactory->CreateTextFormat(
			m_FontName.c_str(),			// font family name
			NULL,					// font collection
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"en-us",
			&m_TextFormat
		);

		hr = m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		hr = m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		GetClientRect(gfx->GetWindowHWND(), &rc);

		hr = m_D2DContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_Brush
		);

		FLOAT dpiX, dpiY;
		gfx->GetFactory()->GetDesktopDpi(&dpiX, &dpiY);

		float dpiScaleX = dpiX / 96.0f;
		float dpiScaleY = dpiY / 96.0f;

		layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.top) / dpiScaleY,
			static_cast<FLOAT>(rc.right - rc.left) / dpiScaleX,
			static_cast<FLOAT>(rc.bottom - rc.top) / dpiScaleY
		);

	}

	cText::~cText()
	{
		if (m_D2DContext) { m_D2DContext->Release(); };
		if (m_TextFactory) { m_TextFactory->Release(); };
		if (m_TextFormat) { m_TextFormat->Release(); };
		if (m_Brush) { m_Brush->Release(); };
	}

	void cText::Draw()
	{
		m_D2DContext->DrawTextA(
			m_WszText_.c_str(),
			m_cTextLength,
			m_TextFormat,
			layoutRect,
			m_Brush
		);
	}

	void cText::Draw(std::wstring text, int x, int y)
	{
		SetText(text);

		SetPosition(x, y);

		Draw();
	}

	void cText::Draw(std::wstring text, sInt2 xy)
	{
		SetText(text);

		SetPosition(xy);

		Draw();
	}

	void cText::Draw(int x, int y)
	{
		SetPosition(x, y);

		Draw();
	}

	void cText::Draw(sInt2 xy)
	{
		SetPosition(xy);

		Draw();
	}

	void cText::Draw(sFloat2 xy)
	{
		SetPosition(xy);

		Draw();
	}

	void cText::SetColour(sColour colour)
	{
		m_Brush->SetColor(D2D1_COLOR_F{ colour.r, colour.g, colour.b, colour.a });
	}

	void cText::SetPosition(int x, int y)
	{
		layoutRect = D2D1::RectF(
			static_cast<FLOAT>(layoutRect.left) - x,
			static_cast<FLOAT>(layoutRect.top) - y,
			static_cast<FLOAT>(layoutRect.right - rc.left) + x,
			static_cast<FLOAT>(layoutRect.bottom - rc.top) + y);
	}

	void cText::SetPosition(sInt2 xy)
	{
		layoutRect = D2D1::RectF(
			static_cast<FLOAT>(layoutRect.left) - xy.x,
			static_cast<FLOAT>(layoutRect.top) - xy.y,
			static_cast<FLOAT>(layoutRect.right - rc.left) + xy.x,
			static_cast<FLOAT>(layoutRect.bottom - rc.top) + xy.y);
	}

	void cText::SetPosition(sFloat2 xy)
	{
		layoutRect = D2D1::RectF(
			static_cast<FLOAT>(layoutRect.left) + xy.x,
			static_cast<FLOAT>(layoutRect.top) + xy.y,
			static_cast<FLOAT>(layoutRect.right - rc.left) + xy.x,
			static_cast<FLOAT>(layoutRect.bottom - rc.top) + xy.y);
	}

	void cText::SetText(std::wstring text)
	{
		m_WszText_ = text;
		m_cTextLength = (UINT32)wcslen(m_WszText_.c_str());
	}
}