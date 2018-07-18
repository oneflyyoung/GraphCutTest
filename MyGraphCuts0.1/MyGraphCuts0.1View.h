
// MyGraphCuts0.1View.h : CMyGraphCuts01View ��Ľӿ�
//

#pragma once


#include "imageholder.h"
#include "MaskManager.h"
#include "GrabcutModel.h"


class CMyGraphCuts01View : public CView
{
protected: // �������л�����
	CMyGraphCuts01View();
	DECLARE_DYNCREATE(CMyGraphCuts01View)

// ����
public:
	CMyGraphCuts01Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
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

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
//��д
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

#ifndef _DEBUG  // MyGraphCuts0.1View.cpp �еĵ��԰汾
inline CMyGraphCuts01Doc* CMyGraphCuts01View::GetDocument() const
   { return reinterpret_cast<CMyGraphCuts01Doc*>(m_pDocument); }
#endif

