
// MyGraphCuts0.1View.h : CMyGraphCuts01View 类的接口
//

#pragma once


#include "imageholder.h"
#include "MaskManager.h"
#include "GrabcutModel.h"


class CMyGraphCuts01View : public CView
{
protected: // 仅从序列化创建
	CMyGraphCuts01View();
	DECLARE_DYNCREATE(CMyGraphCuts01View)

// 特性
public:
	CMyGraphCuts01Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMyGraphCuts01View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	int			 m_width;
	int			 m_height;
	BYTE*		 m_image;
	bool		 m_isDraw;
	int			 m_scale;
	CImageHolder m_swapBuffer;
	CImageHolder m_originalBuffer;
	CImageHolder m_state1Buffer;
	CImageHolder m_state2Buffer;
	CImageHolder* m_stateBuffer;
	CGrabcutModel m_graphModel;
	CMaskManager m_maskManager;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
//重写
public:
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGraphcut();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnChangeLambda();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // MyGraphCuts0.1View.cpp 中的调试版本
inline CMyGraphCuts01Doc* CMyGraphCuts01View::GetDocument() const
   { return reinterpret_cast<CMyGraphCuts01Doc*>(m_pDocument); }
#endif

