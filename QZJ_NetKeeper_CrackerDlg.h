// QZJ_NetKeeper_CrackerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CQZJ_NetKeeper_CrackerDlg 对话框
class CQZJ_NetKeeper_CrackerDlg : public CDialog
{
private:
    static UINT Login(CString,CString);
    static UINT HeartRun(LPVOID);
    static UINT UpdateProgram(LPVOID);
    static UINT PreLogin(LPVOID pParam);
	static CString GetParm(CString data,CString parm);
	static UINT CheckProgram(LPVOID pParam);
	static UINT DownloadPatch(LPVOID pParam);
	CString HashString(CString yourString);
	void SetAutoRun(bool isactive);
    CString Logout();
// 构造
public:
    BOOL ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags);
	CQZJ_NetKeeper_CrackerDlg(CWnd* pParent = NULL);	// 标准构造函数
    void OnClose();
// 对话框数据
	enum { IDD = IDD_QZJ_NETKEEPER_CRACKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
    afx_msg LRESULT onShowTask(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CString m_username;
    CString m_password;
    afx_msg void OnBnClickedButtonlogin();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedCheckremember();
    CButton m_LoginBtn;
    CString m_updateInfo;
    BOOL m_isRemember;
    afx_msg void OnStnClickedStaticupdateinfo();
    afx_msg void On32771();
    afx_msg void On32773();
    afx_msg void On32774();
    afx_msg void On32772();
    afx_msg void OnUpdate();
    afx_msg void On32778();
    afx_msg void On32777();
	afx_msg void OnBnClickedCommand1();
	CButton m_cuteBtn;
	afx_msg void On32798();
	BOOL m_isAutoRun;
	afx_msg void OnBnClickedCheckautorun();
	afx_msg void On32799();
	afx_msg void On32801();
	afx_msg void On32803();
	afx_msg void On32805();
	afx_msg void OnMenu32804();
protected:
	CString m_resultname;
};
