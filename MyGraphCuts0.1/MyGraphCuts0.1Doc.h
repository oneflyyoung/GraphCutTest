
// MyGraphCuts0.1Doc.h : CMyGraphCuts01Doc 类的接口
//


#pragma once


//#include "cv.h"
//#include "cxcore.h"
//#include "highgui.h"

#include "CvvImage.h"


class CMyGraphCuts01Doc : public CDocument
{
protected: // 仅从序列化创建
	CMyGraphCuts01Doc();
	DECLARE_DYNCREATE(CMyGraphCuts01Doc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CMyGraphCuts01Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	bool m_IsFileLoad;

public:
	IplImage * m_imagesrc;
	CvvImage  m_image;

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
