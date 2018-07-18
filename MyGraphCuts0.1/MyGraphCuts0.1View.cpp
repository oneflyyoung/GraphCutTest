
// MyGraphCuts0.1View.cpp : CMyGraphCuts01View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyGraphCuts0.1.h"
#endif

#include "MyGraphCuts0.1Doc.h"
#include "MyGraphCuts0.1View.h"
#include "MainFrm.h"

#include "imageManipulation.h"

#include "SettingDlg.h"
#include "CvvImage.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ComputeTime.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef DELETE_SAFE
#define DELETE_SAFE(x) {if ((x)) delete [](x); (x)=NULL;}
#endif

// CMyGraphCuts01View

IMPLEMENT_DYNCREATE(CMyGraphCuts01View, CView)

BEGIN_MESSAGE_MAP(CMyGraphCuts01View, CView)
	// 标准打印命令
	//ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyGraphCuts01View::OnFilePrintPreview)
	//ON_WM_CONTEXTMENU()
	//ON_WM_RBUTTONUP()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()

	ON_COMMAND(ID_BUTTON_ZOOMIN, OnZoomIn)
	ON_COMMAND(ID_BUTTON_ZOOMOUT, OnZoomOut)

	ON_COMMAND(ID_GRAPHCUT, OnGraphcut)
	ON_COMMAND(ID_SETTING_LAMBDA, OnChangeLambda)

	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMyGraphCuts01View 构造/析构

CMyGraphCuts01View::CMyGraphCuts01View()
{
	// TODO: 在此处添加构造代码

	m_image = NULL;
	m_scale = 1;
	m_isDraw = false;
	m_stateBuffer = &m_state1Buffer;

}

CMyGraphCuts01View::~CMyGraphCuts01View()
{

	DELETE_SAFE(m_image);
}

BOOL CMyGraphCuts01View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式


	return CView::PreCreateWindow(cs);
}



// CMyGraphCuts01View 绘制

void CMyGraphCuts01View::OnDraw(CDC* pDC/*pDC*/)
{
	CMyGraphCuts01Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	m_stateBuffer->drawDC(m_swapBuffer.GetDC(),0,0);
	m_swapBuffer.strechDC(pDC,0,0,m_scale);

	

	// TODO: add draw code for native data here

	// TODO: 在此处为本机数据添加绘制代码
}


//消息处理

void CMyGraphCuts01View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	

	CMyGraphCuts01Doc* pDoc = GetDocument();
	if (pDoc->m_IsFileLoad) 
	{
		CString file = pDoc->GetPathName();
		CvvImage img;

		if (!img.Load(file))
		{
			MessageBox("无法打开文件",NULL, MB_ICONSTOP|MB_OK);
			pDoc->m_IsFileLoad = false;
			return;
		}
		m_width = img.Width();
		m_height = img.Height();
		m_originalBuffer.initialize(m_width, m_height);
		m_state1Buffer.initialize(m_width, m_height);
		m_state2Buffer.initialize(m_width, m_height);
		m_swapBuffer.initialize(m_width, m_height);

		CRect rect(0,0,m_width,m_height);
		img.DrawToHDC(m_originalBuffer.GetDC()->m_hDC,rect);
		img.DrawToHDC(m_state1Buffer.GetDC()->m_hDC,rect);
		img.DrawToHDC(m_state2Buffer.GetDC()->m_hDC,rect);


		CFrameWnd* pFrame = GetParentFrame();
		pFrame->CalcWindowRect(rect);
		rect.MoveToXY(0,0);
		pFrame->MoveWindow(rect);

		int size = m_width * img.Height() * 3;
		DELETE_SAFE(m_image);

		m_image = new BYTE[size];
		BYTE* src = (BYTE*) img.GetImage()->imageData;
		int widthStep = img.GetImage()->widthStep;
		int width3 = m_width * 3;
		for (int i=0;i<m_height;i++) 
		{
			for (int j=0;j<width3;j++)
				m_image[i*width3+j] = src[i*widthStep+j];
		}

		m_maskManager.initMask(m_width, m_height, CGrabcutModel::UND );
		m_graphModel.setImage(m_image, m_width, m_height );
		m_graphModel.setMask(m_maskManager.getMask());

		SetTimer(1, 1000, NULL);
		//周期性触发
	}
	

}


// CMyGraphCuts01View 打印


// void CMyGraphCuts01View::OnFilePrintPreview()
// {
// #ifndef SHARED_HANDLERS
// 	AFXPrintPreview(this);
// #endif
// }

// BOOL CMyGraphCuts01View::OnPreparePrinting(CPrintInfo* pInfo)
// {
// 	// 默认准备
// 	return DoPreparePrinting(pInfo);
// }
// 
// void CMyGraphCuts01View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: 添加额外的打印前进行的初始化过程
// }
// 
// void CMyGraphCuts01View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
// {
// 	// TODO: 添加打印后进行的清理过程
// }



void CMyGraphCuts01View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMyGraphCuts01View 诊断

#ifdef _DEBUG
void CMyGraphCuts01View::AssertValid() const
{
	CView::AssertValid();
}

void CMyGraphCuts01View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyGraphCuts01Doc* CMyGraphCuts01View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyGraphCuts01Doc)));
	return (CMyGraphCuts01Doc*)m_pDocument;
}
#endif //_DEBUG


// CMyGraphCuts01View 消息处理程序


void CMyGraphCuts01View::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_isDraw = true;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CMyGraphCuts01View::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CMyGraphCuts01View::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_isDraw = true;
	//The return value is a handle to the window that had previously captured the mouse.
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CMyGraphCuts01View::OnRButtonUp(UINT nFlags, CPoint point)
{

	CView::OnRButtonUp(nFlags, point);
}

void CMyGraphCuts01View::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);


	point.x /= m_scale;
	point.y /= m_scale;
	if (! m_isDraw) 
		return;
	else if (!(nFlags & MK_LBUTTON) && !(nFlags & MK_RBUTTON) )
	{
		ReleaseCapture();
		m_isDraw = false;
	} else if (nFlags & MK_LBUTTON) 
	{
		m_state1Buffer.setPixel(point.x, point.y, 5, RGB(0,0,255));
		m_state2Buffer.setPixel(point.x, point.y, 5, RGB(0,0,255));
		m_maskManager.setMask(point.x, point.y, 5, CGrabcutModel::OBJ);
	} else 
	{
		m_state1Buffer.setPixel(point.x, point.y, 5, RGB(255,0,0));
		m_state2Buffer.setPixel(point.x, point.y, 5, RGB(255,0,0));
		m_maskManager.setMask(point.x, point.y, 5, CGrabcutModel::BKG);
	}

	Invalidate(0);



}

void CMyGraphCuts01View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent) 
	{
	case 1:
		if (m_stateBuffer == &m_state1Buffer)
			m_stateBuffer = &m_state2Buffer;
		else
			m_stateBuffer = &m_state1Buffer;
		Invalidate(0);
		break;
	default:
		CView::OnTimer(nIDEvent);
	}

}



HBRUSH CMyGraphCuts01View::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	//在MFC类库提供了CWnd::OnCtlColor函数,在工作框架的子窗口被重画时将调用该成员函数
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}



void CMyGraphCuts01View::OnZoomIn()
{
	if (m_scale < 5)
		m_scale ++;
	CRect rect;	
	CFrameWnd* pFrame = GetParentFrame();
	pFrame->GetWindowRect(rect);
	rect.right = rect.left + m_width * m_scale;
	rect.bottom = rect.top + m_height * m_scale;
	pFrame->CalcWindowRect(rect);
	pFrame->MoveWindow(0,0,rect.Width(), rect.Height());	
	Invalidate(0);
}

void CMyGraphCuts01View::OnZoomOut()
{
	if (m_scale > 1)
		m_scale --;
	CRect rect;	
	CFrameWnd* pFrame = GetParentFrame();
	pFrame->GetWindowRect(rect);
	rect.right = rect.left + m_width * m_scale;
	rect.bottom = rect.top + m_height * m_scale;
	pFrame->CalcWindowRect(rect);
	pFrame->MoveWindow(0,0,rect.Width(), rect.Height());	
	Invalidate(0);
}


void CMyGraphCuts01View::OnChangeLambda()
{
	CSettingDlg dlg;
	dlg.m_lambda = m_graphModel.m_lamda;
	if (dlg.DoModal() == IDOK) 
	{
		m_graphModel.m_lamda = dlg.m_lambda;
	}

}


void CMyGraphCuts01View::OnGraphcut()
{
	 ComputeTime ct;
	 ct.Begin();
	
	BYTE* res = m_graphModel.doCut();
	double end = ct.End();
	m_originalBuffer.drawDC(m_state1Buffer.GetDC(), 0, 0);
	m_originalBuffer.drawDC(m_state2Buffer.GetDC(), 0, 0);

	cv::Mat image_contour (cv::Size(m_width, m_height),CV_8UC1);

	BYTE* res1 = new BYTE[m_width*m_height];
	openContour(res, m_width, m_height, res1);
	for (int i=0;i<m_height;i++)
	{
		for (int j=0; j<m_width;j++)
		{
			if (res[i*m_width+j] == 1) 
			{				
				//m_state1Buffer.setPixel(j,i,0,RGB(0,0,100));	//0,0,100大概是蓝色
				//m_state2Buffer.setPixel(j,i,0,RGB(0,0,0));
				image_contour.at<char>(i,j) = res[i*m_width+j];
				
			}
			else
			{
				image_contour.at<char>(i,j) = 255;//让背景显示为白色
			}
		}
	}

	 //2014.7.1增加显示轮廓信息
	cv::namedWindow("image_contour",CV_WINDOW_AUTOSIZE);
	cv::imshow("image_contour",image_contour);
	cv::imwrite("image_contour.bmp",image_contour);
	
	//2014.8.10增加状态栏显示图像分割耗时的代码
	CString str;
	str.Format("%f",end);
	 
	CMainFrame*   pFrame   =   (CMainFrame*)   AfxGetMainWnd(); 
	CMFCStatusBar*   pStatusBar   =   (CMFCStatusBar*)   pFrame-> GetDescendantWindow(AFX_IDW_STATUS_BAR);


	str = "分割用时： "+ str + " ms ";
	pStatusBar->SetPaneText(1, str,TRUE);
	/////////////////////////////////////////////////////
	 
	 delete [] res1;
	Invalidate(0);
}




