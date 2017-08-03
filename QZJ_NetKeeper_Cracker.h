// QZJ_NetKeeper_Cracker.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CQZJ_NetKeeper_CrackerApp:
// 有关此类的实现，请参阅 QZJ_NetKeeper_Cracker.cpp
//

class CQZJ_NetKeeper_CrackerApp : public CWinApp
{
public:
	CQZJ_NetKeeper_CrackerApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CQZJ_NetKeeper_CrackerApp theApp;