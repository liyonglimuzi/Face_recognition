
// MFCDATA.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include"Cconnection.h"


// CMFCDATAApp: 
// �йش����ʵ�֣������ MFCDATA.cpp
//

class CMFCDATAApp : public CWinApp
{
public:
	CMFCDATAApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCDATAApp theApp;