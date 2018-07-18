#pragma once

class CImageHolder
{
public:
	CImageHolder(void);
	~CImageHolder(void);
	int initialize(int Width, int Height);
	
public:
	inline CDC* GetDC(void) {return &m_DC;}

	inline void fillSolid(COLORREF clr) 
	{m_DC.FillSolidRect(0,0,m_Width,m_Height,clr);}

	inline void drawDC(CDC* pDC, int x, int y, DWORD dwRop = SRCCOPY)//最后这个常量是个啥意思？
	{ pDC->BitBlt(x,y,m_Width,m_Height,&m_DC,0,0,dwRop); }

	inline void strechDC(CDC* pDC, int x, int y, int scale, DWORD dwRop = SRCCOPY)
	{ pDC->StretchBlt(x,y,m_Width*scale,m_Height*scale,&m_DC,0,0,m_Width, m_Height, dwRop); }

	void setPixel(int x, int y, int size, COLORREF color);

private:
	CDC m_DC;
	CBitmap m_Bitmap;
	int m_Width;
	int m_Height;

};
