#include "../Headers/TextMgr.h"

TextMgr::TextMgr(D2DMgr* d2dMgr)
{
	m_pD2DMgr = d2dMgr;

	// create standard brushes
	ID2D1SolidColorBrush* yellowBrush;
	ID2D1SolidColorBrush* blackBrush;
	ID2D1SolidColorBrush* whiteBrush;

	if (SUCCEEDED(m_pD2DMgr->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &blackBrush)))
		m_vpBrushes.push_back(blackBrush);

	if (SUCCEEDED(m_pD2DMgr->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &whiteBrush)))
		m_vpBrushes.push_back(whiteBrush);

	if (SUCCEEDED(m_pD2DMgr->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &yellowBrush)))
		m_vpBrushes.push_back(yellowBrush);

	//setup default text format
	AddTextFormat(L"Lucida Console", nullptr, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"en-GB");
}

TextMgr::~TextMgr()
{
}

void TextMgr::Release()
{
	m_pD2DMgr = nullptr;

	for (int i = 0; i < m_vpBrushes.size(); i++)
	{
		SAFE_RELEASE(m_vpBrushes[i]);
		
	}

	m_vpBrushes.clear();

	for (int i = 0; i < m_vpTextFormat.size(); i++)
	{
		SAFE_RELEASE(m_vpTextFormat[i]);
	}

	m_vpTextFormat.clear();

	for (int i = 0; i < m_vTextLayoutVec.size(); i++)
	{
		SAFE_RELEASE(m_vTextLayoutVec[i].textLayout);
	}

	m_vTextLayoutVec.clear();
}

void TextMgr::AddTextFormat(WCHAR const* fontFamilyName, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, WCHAR const* localeName, DWRITE_TEXT_ALIGNMENT txtAlign, DWRITE_PARAGRAPH_ALIGNMENT parAlign)
{
	IDWriteTextFormat* textFormat = nullptr;

	if (SUCCEEDED(m_pD2DMgr->GetWriteFactory()->CreateTextFormat(fontFamilyName, fontCollection, fontWeight, fontStyle, fontStretch, fontSize, localeName, &textFormat)))
	{
		if (SUCCEEDED(textFormat->SetTextAlignment(txtAlign)))
		{
			if (SUCCEEDED(textFormat->SetParagraphAlignment(parAlign)))
			{
				m_vpTextFormat.push_back(textFormat);
			}
		}
	}
}

IDWriteTextLayout* TextMgr::SetupTextLayout(const wchar_t* text, D2D1_POINT_2F origin, D2D1::Matrix3x2F mat)
{
	//text stuffs--------------------------------------------------------------------------
	D2D1_SIZE_F size = m_pD2DMgr->GetDeviceContext()->GetSize();

	TextLayout textLayout;

	//IDWriteTextLayout* textLayout;
	m_vTextLayoutVec.push_back(textLayout);
	m_vTextLayoutVec.back().origin = origin;
	m_vTextLayoutVec.back().mat = mat;
	
	//Direct Write example-----------------------------------------------------------------------------------------------------------------------------------
	//create a DirectWrite Text layout object
	m_pD2DMgr->GetWriteFactory()->CreateTextLayout(text, wcslen(text), m_vpTextFormat[Default], size.width, size.height, &m_vTextLayoutVec.back().textLayout);

	return m_vTextLayoutVec.back().textLayout;
}

void TextMgr::DrawTextLayout(int id)
{
	m_pD2DMgr->GetDeviceContext()->BeginDraw();
	
	m_pD2DMgr->GetDeviceContext()->SetTransform(m_vTextLayoutVec[id].mat);

	m_pD2DMgr->GetDeviceContext()->DrawTextLayout(m_vTextLayoutVec[id].origin, m_vTextLayoutVec[id].textLayout, m_vpBrushes[Black]);
	
	m_pD2DMgr->GetDeviceContext()->EndDraw();
}
