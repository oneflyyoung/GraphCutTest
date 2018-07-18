#pragma once


class CImageProxy
{
public:
	CImageProxy(BYTE* image) {m_image = image;}
	~CImageProxy(void);
private:
	BYTE* m_image;
public:
	inline tagCOLOR operator [] (int i) { return *((tagCOLOR*)(m_image+i*sizeof(tagCOLOR))); }
};
