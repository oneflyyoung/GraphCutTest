
// MyGraphCuts0.1.h : MyGraphCuts0.1 Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMyGraphCuts01App:
// �йش����ʵ�֣������ MyGraphCuts0.1.cpp
//

class CMyGraphCuts01App : public CWinAppEx
{
public:
	CMyGraphCuts01App();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	//��д
public:
	
};

extern CMyGraphCuts01App theApp;
