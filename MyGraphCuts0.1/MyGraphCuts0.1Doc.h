
// MyGraphCuts0.1Doc.h : CMyGraphCuts01Doc ��Ľӿ�
//


#pragma once


//#include "cv.h"
//#include "cxcore.h"
//#include "highgui.h"

#include "CvvImage.h"


class CMyGraphCuts01Doc : public CDocument
{
protected: // �������л�����
	CMyGraphCuts01Doc();
	DECLARE_DYNCREATE(CMyGraphCuts01Doc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CMyGraphCuts01Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	bool m_IsFileLoad;

public:
	IplImage * m_imagesrc;
	CvvImage  m_image;

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
