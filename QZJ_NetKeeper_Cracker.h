// QZJ_NetKeeper_Cracker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQZJ_NetKeeper_CrackerApp:
// �йش����ʵ�֣������ QZJ_NetKeeper_Cracker.cpp
//

class CQZJ_NetKeeper_CrackerApp : public CWinApp
{
public:
	CQZJ_NetKeeper_CrackerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQZJ_NetKeeper_CrackerApp theApp;