#pragma once
#include "afxwin.h"
#include "SocketManager.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

//#define DEBUG
// CNetTalk dialog

class CNetTalk : public CDialog
{
public:
	CNetTalk(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNetTalk();
	static UINT KeepAlive(LPVOID pParam);
// Dialog Data
	enum { IDD = IDD_DIALCHAT };

protected:
	CSocketManager m_SocketManager;
	CString HashString(CString yourString);
	SockAddrIn		m_SockPeer;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void OnBtnDisconnect() ;
	void OnBtnConnect();
	void OnBtnSend();

	DECLARE_MESSAGE_MAP()
public:
	NOTIFYICONDATA  *nid;

	CString m_strPort;
	CString m_username;
	CString m_version;
	CString strServer;
	CEdit m_messageList;
	CEdit m_messagePreSend;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnlogin();
	afx_msg void OnBnClickedBtnexit();
	afx_msg void OnBnClickedBtnbird();
	afx_msg void OnClose(void);
public:
	afx_msg void OnBnClickedBtnchange();

	CString m_nickname;
	CString m_msgValue;
	CString m_txtnickname;
};
