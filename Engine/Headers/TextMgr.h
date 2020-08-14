#ifndef TextMgr_H
#define TextMgr_H

#include "../Headers/D2DMgr.h"

#include <vector>
using namespace std;

enum TextFormatName
{
	Default
};

enum Brushes
{
	Black, White, Yellow
};

struct TextLayout
{
	D2D1_POINT_2F origin;
	D2D1::Matrix3x2F mat;
	IDWriteTextLayout* textLayout;
};

class TextMgr
{
public:
	TextMgr(D2DMgr* d2dMgr);
	~TextMgr();
	void Release();
	void AddTextFormat(WCHAR const* fontFamilyName, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, WCHAR const* localeName,
		DWRITE_TEXT_ALIGNMENT txtAlign = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT parAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	IDWriteTextLayout* SetupTextLayout(const wchar_t* Text, D2D1_POINT_2F origin, D2D1::Matrix3x2F mat);
	void DrawTextLayout(int id);
private:
	D2DMgr* m_pD2DMgr;
	vector<ID2D1SolidColorBrush*> m_vpBrushes;
	vector<IDWriteTextFormat*> m_vpTextFormat;
	vector<TextLayout> m_vTextLayoutVec;
};



#endif // !TextMgr_H
