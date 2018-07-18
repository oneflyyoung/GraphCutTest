
// MyGraphCuts0.1Doc.cpp : CMyGraphCuts01Doc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyGraphCuts0.1.h"
#endif

#include "MyGraphCuts0.1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyGraphCuts01Doc

IMPLEMENT_DYNCREATE(CMyGraphCuts01Doc, CDocument)

BEGIN_MESSAGE_MAP(CMyGraphCuts01Doc, CDocument)
END_MESSAGE_MAP()


// CMyGraphCuts01Doc 构造/析构

CMyGraphCuts01Doc::CMyGraphCuts01Doc()
{
	// TODO: 在此添加一次性构造代码

}

CMyGraphCuts01Doc::~CMyGraphCuts01Doc()
{
}

BOOL CMyGraphCuts01Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMyGraphCuts01Doc 序列化

void CMyGraphCuts01Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMyGraphCuts01Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMyGraphCuts01Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMyGraphCuts01Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMyGraphCuts01Doc 诊断

#ifdef _DEBUG
void CMyGraphCuts01Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyGraphCuts01Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyGraphCuts01Doc 命令


BOOL CMyGraphCuts01Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_IsFileLoad = true;

	m_imagesrc = cvLoadImage(lpszPathName,1);                                                  
	m_image.CopyOf(m_imagesrc,3);

	return TRUE;
}
