// NetTalk.cpp : implementation file
//

#include "stdafx.h"
#include "QZJ_NetKeeper_Cracker.h"
#include "NetTalk.h"
#include "MD5Checksum.h"
#include "MD5ChecksumDefines.h"
#include "VirtualizerSDK.h"
#include "Crypt.h"
#include "NetKeeperHeader.h"

bool isConnented;


CNetTalk::CNetTalk(CWnd* pParent /*=NULL*/)
	 : CDialog(CNetTalk::IDD)
	 , m_nickname(_T(""))
	 , m_msgValue(_T(""))
	 , m_txtnickname(_T(""))
{

}

CNetTalk::~CNetTalk()
{
}

void CNetTalk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_LIST, m_messageList);
	DDX_Control(pDX, IDC_MESSAGE_SEND, m_messagePreSend);
	//DDX_Text(pDX, IDC_EDITNICKNAME, m_nickname);
	DDX_Text(pDX, IDC_MESSAGE_SEND, m_msgValue);
	DDX_Text(pDX, IDC_EDITNICKNAME, m_txtnickname);
}

BOOL CNetTalk::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SocketManager.SetMessageWindow( &m_messageList );
	m_SocketManager.SetServerState( false );	// run as client
	m_SocketManager.SetSmartAddressing( false );	// always send to server
	OnBtnConnect();
	AfxBeginThread(KeepAlive,this);
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CNetTalk, CDialog)
	ON_WM_CLOSE()
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEND, &CNetTalk::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTNLOGIN, &CNetTalk::OnBnClickedBtnlogin)
	ON_BN_CLICKED(IDC_BTNEXIT, &CNetTalk::OnBnClickedBtnexit)
	ON_BN_CLICKED(IDC_BTNBIRD, &CNetTalk::OnBnClickedBtnbird)
	ON_BN_CLICKED(IDC_BTNCHANGE, &CNetTalk::OnBnClickedBtnchange)
	//ON_MESSAGE(WM_SHOWCHATWIN
END_MESSAGE_MAP()

void CNetTalk::OnBtnConnect() 
{
	VIRTUALIZER_START
	UpdateData();
	if (m_strPort == "")
		m_strPort = "85";
	if (strServer == "")
		strServer = "nov30th.com";
	bool bSuccess;
	bSuccess = m_SocketManager.ConnectTo( strServer, m_strPort, AF_INET, SOCK_STREAM); // TCP

	if (bSuccess && m_SocketManager.WatchComm())
	{

		//GetDlgItem(IDC_BTNLOGIN)->EnableWindow( FALSE );
		PrevDlgCtrl();
		GetDlgItem(IDC_BTN_SEND)->EnableWindow( TRUE );
		//GetDlgItem(IDC_BTNEXIT)->EnableWindow( TRUE );
		CString strMsg;

		strMsg = _T("杭电极速(代号XL闪讯)\r\n ");
		m_SocketManager.GetPeerName( m_SockPeer );
		m_messageList.SetWindowText( strMsg );

		/////////////////////////////////////////////////////SEND HASH
		CString strText;
		strText = m_nickname + "[" + m_version + "]";
		strText = "|NAME:" + strText + "|Verify:" + HashString(strText) + "|";
		strText = "[LOGIN]" + strText;
		BYTE byBuffer[256] = { 0 };
		int nLen = strText.GetLength();
		if (nLen > 0)
		{
			//strText += _T("\r\n");
			nLen = strText.GetLength() + 1;
			USES_CONVERSION;
			strcpy((LPSTR)byBuffer, T2CA(strText));
			m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
		}


		
		/*
		CString tempHASH;
		CCrypt crypt;
		tempHASH = m_nickname + "|" + m_version;
		tempHASH = crypt.Encrypt(tempHASH,1);
		tempHASH = "QZJ" + tempHASH + "QZJ";
		tempHASH = crypt.Encrypt(tempHASH,2);
		int nLen = tempHASH.GetLength() + 1;
		USES_CONVERSION;
		BYTE byBuffer[256] = {0};
		strcpy((LPSTR)byBuffer, T2CA(tempHASH));
		m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
		*/
	}
	else
	{
		AfxMessageBox("连接失败!");
	}
	VIRTUALIZER_END
	
}


void CNetTalk::OnBtnDisconnect() 
{
	// Disconnect socket
	m_SocketManager.StopComm();
	if (!m_SocketManager.IsOpen())
	{
		//GetDlgItem(IDC_BTNLOGIN)->EnableWindow( TRUE );
		PrevDlgCtrl();
		GetDlgItem(IDC_BTN_SEND)->EnableWindow( FALSE );
		//GetDlgItem(IDC_BTNEXIT)->EnableWindow( FALSE );

	}
}

void CNetTalk::OnBtnSend() 
{
	VIRTUALIZER_START
	if (!m_SocketManager.IsOpen())
	{
		AfxMessageBox("未连接");
		return;
	}
	BYTE byBuffer[256] = { 0 };
	CString strText;

	CMD5Checksum md5;
	char md5Temp[200];
	strcpy(md5Temp,m_nickname);
	strcat(md5Temp,"!@#$%^&*()(*&^%$#@!");
	strcat(md5Temp,"41");
#ifndef DEBUG
	if (m_username != md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp)))
	{
		AfxMessageBox("奇怪了.名字不一样类...");
		return;
	}
#endif

	// Update so that we can retrieve new port number also... :-)
	UpdateData();
	m_messagePreSend.GetWindowText( strText );

	if (strText.GetLength() < 1)
		return;
	if (strText.GetLength() > 100)
	{
		AfxMessageBox("请小于100字~");
		return;
	}
	int nLen = strText.GetLength();
	if (nLen > 0)
	{
		//strText += _T("\r\n");
		nLen = strText.GetLength() + 1;
		USES_CONVERSION;
		strcpy((LPSTR)byBuffer, T2CA(strText));
		m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
	}
	m_msgValue = "";
	UpdateData(false);
	VIRTUALIZER_END
}
void CNetTalk::OnBnClickedBtnSend()
{
	OnBtnSend();
}

void CNetTalk::OnBnClickedBtnlogin()
{
	OnBtnConnect();
}

void CNetTalk::OnBnClickedBtnexit()
{
	OnBtnDisconnect();
	Sleep(2000);
	OnBtnConnect();
}

void CNetTalk::OnBnClickedBtnbird()
{
	AfxMessageBox("鸟人！！鸟人！！");
}

void CNetTalk::OnClose(void)
{
	this->ShowWindow(SW_HIDE);
	CWnd* pParent = this->GetParent();
	pParent->SendMessage( WM_SHOWCHATICO,0,2);
}

void CNetTalk::OnBnClickedBtnchange()
{
	VIRTUALIZER_START
	UpdateData();
	if (m_txtnickname.GetLength() > 8 || m_txtnickname.GetLength() < 2)
	{
		AfxMessageBox("名字长度有问题。。");
		return;
	}
	if (m_txtnickname.Find("[")>=0 || m_txtnickname.Find("]")>=0 ||  m_txtnickname.Find("|")>=0 ||  m_txtnickname.Find(":")>=0  ||  m_txtnickname.Find("\\")>=0)
	{
		AfxMessageBox("名字中不能有特殊符号~");
		return;
	}
	CString strText;
	strText = m_txtnickname + "[" + m_version + "]";
	strText = "|NAME:" + strText + "|Verify:" + HashString(strText) + "|";
	strText = "[NAME]" + strText;
	BYTE byBuffer[256] = { 0 };
	int nLen = strText.GetLength();
	if (nLen > 0)
	{
		nLen = strText.GetLength() + 1;
		USES_CONVERSION;
		strcpy((LPSTR)byBuffer, T2CA(strText));
		m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
	}
	VIRTUALIZER_END
}

CString CNetTalk::HashString(CString yourString)
{
	VIRTUALIZER_START
	CMD5Checksum md5;
	char md5Temp[200];
	static CString retval;
	strcpy(md5Temp,yourString);
	strcat(md5Temp,"!@#$%^&*()(*&^%$#@!");
	strcat(md5Temp,"41");
	retval = md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp));
	VIRTUALIZER_END
	return retval;
}

UINT CNetTalk::KeepAlive(LPVOID pParam)
{
	CNetTalk* app = (CNetTalk*)pParam;
	CString strText = "[KeepAlive]";
	BYTE byBuffer[256] = { 0 };
	int nLen = strText.GetLength() + 1;
	USES_CONVERSION;
	strcpy((LPSTR)byBuffer, T2CA(strText));
	while(1)
	{
		Sleep(30000);
		if (!app->m_SocketManager.IsOpen())
		{
			continue;
		}
		app->m_SocketManager.WriteComm( byBuffer, nLen, INFINITE);
	}
	return 0;
}

