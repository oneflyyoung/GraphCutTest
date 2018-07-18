#include "StdAfx.h"
#include "imageholder.h"


int CImageHolder::initialize(int Width, int Height)
{
	CDC dc;
	dc.m_hDC = ::GetDC(NULL);
	m_DC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc, Width, Height);
	m_DC.SelectObject(&m_Bitmap);
	fillSolid(RGB(0,0,0));

	m_Width = Width;
	m_Height = Height;

	return 0;
}

CImageHolder::CImageHolder(void)
{

}

CImageHolder::~CImageHolder(void)
{
}

void CImageHolder::setPixel(int x, int y, int size, COLORREF color) 
{
	int i,j;
	for (i=x-size;i<=x+size;i++)
		for (j=y-size;j<=y+size;j++)
			m_DC.SetPixel(i,j,color);

}