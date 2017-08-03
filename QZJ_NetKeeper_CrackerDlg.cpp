// QZJ_NetKeeper_CrackerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QZJ_NetKeeper_Cracker.h"
#include "QZJ_NetKeeper_CrackerDlg.h"
//#include "winsock.h"
#include <afxinet.h>
#include "Nov30thAuth.h"
#include "TaskbarNotifier.h"
#include "Crypt.h"
#include "ChineseCodeLib.h"
#include "VirtualizerSDK.h"
#include "Md5Checksum.h"
#include "NetKeeperHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define DEBUG

#define PROGRAMVERSION "2008"

CString DZKDXY = "@DZKD.XY";

HICON hIconOffline,hIconOnline,hIconNotLogin,hIconChat;
NOTIFYICONDATA   nid;
static bool isError;
static bool isRun;
CString username,password,usernameMD5;
char buffresult[4096];
CTaskbarNotifier m_wndTaskbarNotifier1;
CString BlockList;
char md5Temp[200] = {0};
bool isChatICO = false;
bool hasUpdate = false;
bool hasChatInit = false;
CString m_pin;
CString patchAddress,patchFileName;
long waitTime = 30000;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_aboutStr;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_aboutStr(_T("������QZJ [HOHO``]ͬѧ������\r\n���޺��ݵ��ӿƼ���ѧ�ڲ��о�ʹ�á�\r\n�����ļ���HOHO���ǩ����\r\n��ֹ�κγ��ԶԱ������򹤳̵���Ϊ��\r\n����׷���������Ρ�"))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_aboutStr);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CQZJ_NetKeeper_CrackerDlg �Ի���




CQZJ_NetKeeper_CrackerDlg::CQZJ_NetKeeper_CrackerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CQZJ_NetKeeper_CrackerDlg::IDD, pParent)
, m_username(_T(""))
, m_password(_T(""))
, m_isRemember(FALSE)
, m_updateInfo(_T("��������������������"))
, m_isAutoRun(FALSE)
, m_resultname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQZJ_NetKeeper_CrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER, m_username);
	DDX_Text(pDX, IDC_PASS, m_password);
	DDX_Check(pDX, IDC_CHECKREMEMBER, m_isRemember);
	DDX_Control(pDX, IDC_BUTTONLOGIN, m_LoginBtn);
	DDX_Check(pDX, IDC_CHECKAUTORUN, m_isAutoRun);
	DDX_Text(pDX, IDC_EDITRESULTNAME, m_resultname);
}

BEGIN_MESSAGE_MAP(CQZJ_NetKeeper_CrackerDlg, CDialog)
	ON_MESSAGE(WM_SHOWTASK,onShowTask)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONLOGIN, &CQZJ_NetKeeper_CrackerDlg::OnBnClickedButtonlogin)
	ON_BN_CLICKED(IDC_CHECKREMEMBER, &CQZJ_NetKeeper_CrackerDlg::OnBnClickedCheckremember)
	ON_COMMAND(ID_32771, &CQZJ_NetKeeper_CrackerDlg::On32771)
	ON_COMMAND(ID_32773, &CQZJ_NetKeeper_CrackerDlg::On32773)
	ON_COMMAND(ID_32774, &CQZJ_NetKeeper_CrackerDlg::On32774)
	ON_COMMAND(ID_32772, &CQZJ_NetKeeper_CrackerDlg::On32772)
	ON_COMMAND(ID_Update, &CQZJ_NetKeeper_CrackerDlg::OnUpdate)
	ON_COMMAND(ID_32778, &CQZJ_NetKeeper_CrackerDlg::On32778)
	ON_COMMAND(ID_32777, &CQZJ_NetKeeper_CrackerDlg::On32777)
	ON_COMMAND(ID_32798, &CQZJ_NetKeeper_CrackerDlg::On32798)
	ON_BN_CLICKED(IDC_CHECKAUTORUN, &CQZJ_NetKeeper_CrackerDlg::OnBnClickedCheckautorun)
	ON_COMMAND(ID_32799, &CQZJ_NetKeeper_CrackerDlg::On32799)
	ON_COMMAND(ID_32801, &CQZJ_NetKeeper_CrackerDlg::On32801)
	ON_COMMAND(ID_32803, &CQZJ_NetKeeper_CrackerDlg::On32803)
END_MESSAGE_MAP()


// CQZJ_NetKeeper_CrackerDlg ��Ϣ�������

BOOL CQZJ_NetKeeper_CrackerDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	char   szTheAppPath[MAX_PATH];   
	::GetModuleFileName(NULL,   szTheAppPath,   MAX_PATH);   
	char   *p   =   strrchr(szTheAppPath,   '\\');   
	*++p   =   0;   
	if (!SetCurrentDirectory(szTheAppPath))
		AfxMessageBox("Change Dir Error.");

	/*
	CCrypt crypt;
	CString address = "http://students.hdu.edu.cn/NetKeeper.Msi";
	CString filename = "NetKeeper.Msi";
	CString address1 = crypt.Encrypt(address,'U');
	filename = crypt.Encrypt(filename,'P');
	CString hash = crypt.Encrypt(address,'D');
	int i = 1;
	*/

	{
		CString cmdString = GetCommandLine();
		if (cmdString.Find(" -QuickResponse") > 0 && m_isRemember==TRUE)
		{
			m_isAutoRun = true;
			waitTime = 10000;
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	m_wndTaskbarNotifier1.Create(this);
	m_wndTaskbarNotifier1.SetSkin(IDB_BITMSN);
	m_wndTaskbarNotifier1.SetTextFont("Arial",90,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
	m_wndTaskbarNotifier1.SetTextColor(RGB(0,0,0),RGB(0,0,200));
	m_wndTaskbarNotifier1.SetTextRect(CRect(10,23,m_wndTaskbarNotifier1.m_nSkinWidth-10,m_wndTaskbarNotifier1.m_nSkinHeight-25));
	VIRTUALIZER_START
		char title[200];
	sprintf_s(title,sizeof(title),"���缫�� [��ɫ��԰ר��] -- Nov30th.Com Software.",PROGRAMVERSION);
	this->SetWindowTextA(title);
	m_wndTaskbarNotifier1.Show("�������Ӻ�ɫ��԰����������!",500,5000,500);
	AfxBeginThread(UpdateProgram,this);
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uCallbackMessage=WM_SHOWTASK;  //�Զ������Ϣ���� 
	//hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	hIconOnline = AfxGetApp()->LoadIcon(IDI_ONLINE);
	hIconOffline = AfxGetApp()->LoadIcon(IDI_OFFLINE);
	hIconNotLogin = AfxGetApp()->LoadIcon(IDI_LOGOUT);
	nid.hIcon = hIconNotLogin;
	strcpy(nid.szTip, "���缫�� [��ɫ��԰���հ�] | HOHO`` Software");
	Shell_NotifyIcon(NIM_ADD ,&nid);
	m_username = "057181293808@DZKD.XY";
	CStdioFile file;
	char temp[100];
	CString filepath;
	CString autoStart;
	DeleteFile(_T("Qzj_NetKeeper_Cracker.cfg"));
	if (file.Open("config.qzj",CFile::modeRead))
	{
		CCrypt *crypt = new CCrypt();
		file.ReadString(temp,100);
		m_username = crypt->Decrypt(temp,31);
		file.ReadString(temp,100);
		m_password = crypt->Decrypt(temp,31);
		file.ReadString(temp,100);
		file.Close();
		m_isRemember = true;
	}
	CString cmdString = GetCommandLine();
	UpdateData(false);
	if (cmdString.Find(" -autorun") > 0 && m_isRemember==TRUE)
	{
		m_isAutoRun = true;
		UpdateData(false);
		//OnBnClickedButtonlogin();
	}
#ifdef DEBUG
	GetDlgItem(IDC_BUTTONCHAT)->EnableWindow(TRUE);
#endif
	VIRTUALIZER_END
		return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQZJ_NetKeeper_CrackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQZJ_NetKeeper_CrackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQZJ_NetKeeper_CrackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQZJ_NetKeeper_CrackerDlg::OnBnClickedButtonlogin()
{

	CNov30thAuth auth;
	CString tempString;
	UpdateData();
	m_resultname = "";
	m_resultname = auth.getPIN(m_username);
	m_resultname += m_username;
	tempString = "http://192.168.0.1/cgi-bin/prim?rc=@prim&rf=0004&rd=x&Wt=0200&lf=0037&ai=0&En=00-19-5B-DB-65-73&_PT=on&IU0=";
	tempString += m_resultname;
	tempString += "&IW0=";
	tempString += m_password;
	tempString += "&_IW1=";
	tempString += m_password;
	tempString += "&IT0=&DT0=300&_DT0=5&IM0=1492&DF00=o&DF01=x&_AD=on&Ef0%23=20&Ef1%23=0&Ef05=x&Ef0%23=2000&Ef1%23=8&Ef13=o";
	UpdateData(false);
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",  tempString,   NULL,   SW_SHOWMAXIMIZED   );
	//URI:   
	//m_LoginBtn.EnableWindow(false);
	//m_cuteBtn.EnableWindow(false);
	//AfxBeginThread(PreLogin, this);
	//m_wndTaskbarNotifier1.Show("��¼�С����Ժ򡣡���",500,5000,500);
}

UINT CQZJ_NetKeeper_CrackerDlg::PreLogin(LPVOID pParam)
{
	VIRTUALIZER_START
		CQZJ_NetKeeper_CrackerDlg* app = (CQZJ_NetKeeper_CrackerDlg*)pParam;
	CNov30thAuth auth;
	app->UpdateData(true);
	if (isRun)
	{
		AfxMessageBox("�뱨�����BUG.һ����ť�޿��ܰ�����!");
		return 1;
	}
	if (app->m_username.GetLength() <12)
		return 1;
	m_pin = auth.getPIN(app->m_username.Left(12));
	/////////////////////////////BLOCK ACTION
	CString blockString = app->HashString(app->m_username.Left(12));
	blockString = blockString.Mid(2,5);
	blockString = blockString + ",";
	if (strstr(BlockList,blockString)!=NULL)
	{
		hasUpdate == true;
	}
	if (BlockList.FindOneOf(blockString) >=0 )
	{
		hasUpdate == true;
	}
	app->UpdateData(false);
	if (app->m_username == _T(""))
	{
		AfxMessageBox(_T("�û���!"));
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		return 1;
	}
	app->m_username.Replace("&","");
	app->m_username.Replace("%","");
	app->m_username.Replace(" ","");
	char hz[20];
	strcpy(hz,DZKDXY);
	if (hz[1]!='E'-1 || hz[4]!=hz[1] || hz[5] != '.' || hz[0]!='@')
	{
		app->m_password = "";
		app->m_username = "";
	}

	if (app->m_username.Find(DZKDXY)<0)
	{
		AfxMessageBox("�ǳ���Ǹ���ʺź�׺�������");
		app->m_username = "@DZKD.XY";
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		app->UpdateData(false);
		return 1;
	}
	app->m_password.Replace("&","");
	app->m_password.Replace("%","");
	app->m_password.Replace(" ","%20");
	if (app->m_password == "")
		return 1;
	//CStdioFile file;
	if (Login(app->m_username,app->m_password)==0)
	{
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		return 1;
	}
	CString result;
	result.Format("%s",buffresult);
	int findPo = result.Find(_T("PROMPT_INFO"),0);
	if (findPo < 0)
	{
		AfxMessageBox(_T("�޷���÷�������Ϣ"));
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		return 1;
	}
	findPo += 12;
	int findPo2 = result.Find(_T("&"),findPo);
	if (findPo2 < 0)
	{
		AfxMessageBox(_T("�޷���÷�������Ϣ"));
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		return 1;
	}
	result.SetAt(findPo2,'\0');
	char rStr[100];
	int i;
	for (i = findPo; i<findPo2;i++)
	{
		rStr[i-findPo] = result.GetAt(i);
	}
	rStr[i-findPo] = '\0';

	if (result.Find("RESULT=0") >0)
	{
		app->m_LoginBtn.EnableWindow(false);
		isRun = true;
		app->OnClose();  
		app->GetDlgItem(IDC_USER)->EnableWindow(FALSE);
		app->GetDlgItem(IDC_PASS)->EnableWindow(FALSE);

		m_wndTaskbarNotifier1.Show("HOHO``��¼�ɹ��������ڼ��벻Ҫ�رա�",500,5000,500);
		nid.hIcon = hIconOnline;
		Shell_NotifyIcon(NIM_MODIFY ,&nid);
		AfxBeginThread(app->HeartRun,app );
	}
	else if (result.Find("RESULT=2")>0)
	{
		if (AfxMessageBox("�������������������˳���������?",(MB_YESNO   |   MB_ICONQUESTION)   + 4096)==IDYES)   
		{
			app->Logout();
		}
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
		return 0;
	}
	else
	{
		AfxMessageBox(rStr);
		app->m_LoginBtn.EnableWindow(true);
		app->m_cuteBtn.EnableWindow(true);
	}
	if (app->m_isRemember)
	{
		CStdioFile file;
		if (file.Open("config.qzj",CFile::modeWrite | CFile::modeCreate)== false)
			AfxMessageBox("�û�������д���ļ�ʧ�ܡ�");
		else
		{
			CCrypt *crypt = new CCrypt();
			file.WriteString(crypt->Encrypt(app->m_username,31));
			file.WriteString("\n");
			file.WriteString(crypt->Encrypt(app->m_password,31));
			file.WriteString("\n");
			file.Close();
		}
	}
	else
		DeleteFile("config.qzj");
	VIRTUALIZER_END
		return 0;
}

UINT CQZJ_NetKeeper_CrackerDlg::Login(CString n_username, CString n_password)
{
	VIRTUALIZER_START
		int errorTimes = 1;
	memset(buffresult,0,4096);
	DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT;  // We are connecting to internet directly
	DWORD dwServiceType;
	CInternetSession session(("Nov30th"),0, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CString strServerName ;
	CString strObject;
	if (hasUpdate == true)
	{
		AfxMessageBox("��ֹ��¼!");
		return 0;
	}
	int retval;
	INTERNET_PORT nPort;
	//InternetSetOption (
	char data[1000];
	sprintf_s(data,"https://portal.114school.cn/imptypt/netkeepercontroller?IP=&MAC=&DRIVER=1&VERSION_NUMBER=1.0.0031&TYPE=LOGIN&USER_NAME=%s&PASSWORD=%s&PIN=%s",n_username,n_password,m_pin);
	AfxParseURL(data, dwServiceType, strServerName, strObject, nPort);// ||   dwServiceType != AFX_INET_SERVICE_HTTPS);
	pServer = session.GetHttpConnection(strServerName, nPort);
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT |
		INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_IGNORE_CERT_CN_INVALID );
	VIRTUALIZER_END
		TCHAR   szErr[1024];
	while(true)
	{
		try
		{;
		pFile->SendRequest();
		break;
		}
		catch(CInternetException *pEx)
		{
			if (errorTimes++ >= 10)
			{			
				pEx->GetErrorMessage(szErr,1023);   
				pEx->Delete();
				AfxMessageBox(szErr);
				return 0;
			}
		}
	}
	VIRTUALIZER_START
		retval = (pFile->Read(buffresult,sizeof(buffresult)));
	pFile->Close();
	username = n_username;
	password = n_password;
	VIRTUALIZER_END
		return 1;
}

UINT CQZJ_NetKeeper_CrackerDlg::HeartRun(LPVOID   pParam   )
{
	if (isRun == false)
		return 0;
	int i=0;
	CQZJ_NetKeeper_CrackerDlg* app = (CQZJ_NetKeeper_CrackerDlg*)pParam;
	while(isRun)
	{

		Sleep(waitTime);
		VIRTUALIZER_START
			memset(buffresult,0,4096);
		DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT;  // We are connecting to internet directly
		DWORD dwServiceType;
		CInternetSession session(("Nov30th"),0, dwAccessType);
		CHttpConnection* pServer = NULL;
		CHttpFile* pFile = NULL;
		CString strServerName ;
		CString strObject;
		INTERNET_PORT nPort;
		//InternetSetOption (
		char data[1000];
		sprintf_s(data,"https://heart.114school.cn:8443/heartServer/portal.do?TYPE=HEARTBEAT&USER_NAME=%s&PASSWORD=%s&DRIVER=1&VERSION_NUMBER=1.0.0031",username,password);
		AfxParseURL(data, dwServiceType, strServerName, strObject, nPort);// ||   dwServiceType != AFX_INET_SERVICE_HTTPS);
		pServer = session.GetHttpConnection(strServerName, nPort);
		//pServer->SetOption(
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT |
			INTERNET_FLAG_RELOAD |
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_SECURE |
			INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
			INTERNET_FLAG_IGNORE_CERT_CN_INVALID );
		VIRTUALIZER_END
			try
		{
			pFile->SendRequest();
		}
		catch(...)
		{
			m_wndTaskbarNotifier1.Show("HOHO``����ŷ�����ͨ�ų��������������ӣ�",500,5000,500);
			nid.hIcon = hIconOffline;
			Shell_NotifyIcon(NIM_MODIFY ,&nid);
			isError = true;
			continue;
		}
		VIRTUALIZER_START
			pFile->Close();
		if (isError)
		{
			nid.hIcon = hIconOnline;
			Shell_NotifyIcon(NIM_MODIFY ,&nid);
			isError = false;
		}
		VIRTUALIZER_END
	}

	return 0;
}       


void CQZJ_NetKeeper_CrackerDlg::OnBnClickedCancel()
{
	Shell_NotifyIcon(NIM_DELETE ,&nid);
	if (isRun)
	{
		isRun = false;
		if   (AfxMessageBox("�˳�����������?",(MB_YESNO   |   MB_ICONQUESTION)   + 4096)!=IDYES)   
		{
			Logout();
		}
	}
	OnCancel();
}

void CQZJ_NetKeeper_CrackerDlg::OnClose()
{
	/*
	if (false)
	{
	Shell_NotifyIcon(NIM_DELETE ,&nid);
	this->DestroyWindow();
	}
	*/
	if (!IsWindowVisible())
	{
		//���ش���,�����Ҫ��ǿ�ƽ���
		OnCancel();
		OnDestroy();
		return;
	}
	this->ShowWindow(SW_HIDE);
}

LRESULT CQZJ_NetKeeper_CrackerDlg::onShowTask(WPARAM wParam,LPARAM lParam) 
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
{ 
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam) 
	{ 
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
		{ 
			CPoint pt;
			CMenu menu;
			menu.LoadMenuA(IDR_MENU2);
			GetCursorPos(&pt);
			//ClientToScreen(&pt);
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,pt.x,pt.y,this);  
		} 
		break; 
	case WM_LBUTTONDBLCLK://˫������Ĵ��� 
		{
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
		}
		break;
	} 
	return 0; 
}

CString CQZJ_NetKeeper_CrackerDlg::Logout()
{
	VIRTUALIZER_START
		memset(buffresult,0,4096);
	DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT;  // We are connecting to internet directly
	DWORD dwServiceType;
	CInternetSession session(("Nov30th"),0, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CString strServerName ;
	CString strObject;
	INTERNET_PORT nPort;
	//InternetSetOption (
	char data[1000];
	sprintf_s(data,"https://portal.114school.cn/imptypt/netkeepercontroller?TYPE=LOGOUT&USER_NAME=%s&PASSWORD=%s",m_username,m_password);
	AfxParseURL(data, dwServiceType, strServerName, strObject, nPort);// ||   dwServiceType != AFX_INET_SERVICE_HTTPS);
	pServer = session.GetHttpConnection(strServerName, nPort);
	//pServer->SetOption(
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT |
		INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE |
		INTERNET_FLAG_SECURE |
		INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
		INTERNET_FLAG_IGNORE_CERT_CN_INVALID );
	pFile->SendRequest();
	Sleep(1000);
	pFile->Close();
	VIRTUALIZER_END
		return _T("");
}

void CQZJ_NetKeeper_CrackerDlg::OnBnClickedCheckremember()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
}

CString CQZJ_NetKeeper_CrackerDlg::GetParm(CString result,CString parm)
{
	VIRTUALIZER_START
		CString stringStr;
	stringStr.Format("|%s:",parm);
	int findPo = result.Find(stringStr,0);
	if (findPo < 0)
	{
		return "";
	}
	findPo += stringStr.GetLength();
	int findPo2 = result.Find(_T("|"),findPo);
	if (findPo2 < 0)
	{
		return "";
	}
	result.SetAt(findPo2,'\0');
	char rStr[1024];
	int i;
	for (i = findPo; i<findPo2;i++)
	{
		rStr[i-findPo] = result.GetAt(i);
	}
	rStr[i-findPo] = '\0';
	VIRTUALIZER_END
		return rStr;
}

UINT CQZJ_NetKeeper_CrackerDlg::UpdateProgram(LPVOID pParam)
{
	VIRTUALIZER_START
		CQZJ_NetKeeper_CrackerDlg* app = (CQZJ_NetKeeper_CrackerDlg*)pParam;
	char bbsbuff[409600];

	memset(buffresult,0,4095);
	memset(bbsbuff,0,409600);
	DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT;  // We are connecting to internet directly
	DWORD dwServiceType;
	CInternetSession session(("Nov30th"),0, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CString strServerName ;
	CString strObject;
	INTERNET_PORT nPort;
	//InternetSetOption (
	CString bbspath = "http://192.168.100.153/viewthread.php?tid=343395&extra=page%3D1";
	AfxParseURL(bbspath, dwServiceType, strServerName, strObject, nPort);// ||   dwServiceType != AFX_INET_SERVICE_HTTPS);
	pServer = session.GetHttpConnection(strServerName, nPort);
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL,INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE);
	pFile->AddRequestHeaders("Host: bbs.redhome.cc");

	try
	{
		pFile->SendRequest();
	}		
	catch(...)
	{
		AfxMessageBox("��ɫ��԰����ʧ��.������ֹ!����У��ʹ��!");
		//m_wndTaskbarNotifier1.Show("��ɫ��԰��������������ʧ�ܣ�",500,5000,500);
		//app->m_updateInfo = "��������������ʧ�ܡ�";
		hasUpdate = true;
		return 1;
	}

	while (pFile->Read(buffresult,sizeof(buffresult)))
	{
		strcat_s(bbsbuff,buffresult);
	}

	pFile->Close();
	app->UpdateData(true);


	CString result;
	CString addInfo;
	CString rStr;

	rStr = GetParm(bbsbuff,"CODE_XL");

	addInfo = GetParm(bbsbuff,PROGRAMVERSION);
	if (rStr =="")
	{
		result = "�����汾��ȡʧ�ܡ�";

	}
	else if (rStr ==PROGRAMVERSION)
	{
		result =  "��ϲ.��ɫ��԰���°汾.";
	}
	else if (rStr > PROGRAMVERSION)
	{
		hasUpdate = true;
		result = "�����°汾:";
		result += rStr;
		//////////////update
		rStr = GetParm(bbsbuff,"UPDATEADDRESS");
		if (rStr != "")
		{
			CCrypt crypt;
			patchAddress = crypt.Decrypt(rStr,'U');
			rStr = GetParm(bbsbuff,"FILENAME");
			patchFileName = crypt.Decrypt(rStr,'P');
			rStr = GetParm(bbsbuff,"SAFEHASH");
			CString tempHASH = crypt.Encrypt(patchAddress,'D');
			if (rStr == tempHASH)
			{
				AfxBeginThread(app->DownloadPatch,app );
			}
		}
	}
	else
	{
		result = "���°汾.(�ȷ����汾��)";
	}
	if (addInfo != "")
		result += addInfo;

	////////////////////////////////////////////////////////////CHECKHERE

	BlockList =  GetParm(bbsbuff,"BA");

	////////////////////////////////////////////////////////////
	CString startUpHash = GetParm(bbsbuff,"VA");
	////////////////////////////////////////////////////////////
	if (BlockList == "" || BlockList == "")
	{
		hasUpdate = true;
		return 0;
	}
	rStr = GetParm(bbsbuff,"CODE_XL");
	CMD5Checksum md5;
	strcpy(md5Temp,BlockList);
	strcat(md5Temp,"!@#$%^&*()(*&^%$#@!");
	strcat(md5Temp,"41");
	strcat(md5Temp,rStr);
	usernameMD5 = md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp));
	strcpy(md5Temp,usernameMD5);
	usernameMD5 = md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp));
	strcpy(md5Temp,usernameMD5);
	usernameMD5 = md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp));
	strcpy(md5Temp,usernameMD5);
	usernameMD5 = md5.GetMD5((BYTE*)md5Temp,strlen(md5Temp));
	usernameMD5 = usernameMD5.Mid(13,6);
	if (usernameMD5 != startUpHash)
	{
		hasUpdate = true;
		return 0;
	}

	app->m_updateInfo =  result;
	m_wndTaskbarNotifier1.Show(app->m_updateInfo,500,5000,500);
	app->UpdateData(false);
	if (app->m_isAutoRun)
		app->OnBnClickedButtonlogin();
	VIRTUALIZER_END
		return 0;
}

void CQZJ_NetKeeper_CrackerDlg::OnStnClickedStaticupdateinfo()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://bbs.redhome.cc/viewthread.php?tid=343395&extra=page%3D1",   NULL,   SW_SHOWMAXIMIZED   );
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CQZJ_NetKeeper_CrackerDlg::ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, UINT uTimeout, DWORD dwInfoFlags)
{
	return true;
	nid.cbSize=sizeof(NOTIFYICONDATA);
	nid.uFlags = NIF_INFO;
	nid.uTimeout = uTimeout;
	nid.dwInfoFlags = dwInfoFlags;
	strcpy(nid.szInfo,szMsg ? szMsg : _T(""));
	strcpy(nid.szInfoTitle,szTitle ? szTitle : _T(""));
	return Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void CQZJ_NetKeeper_CrackerDlg::On32771()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://bbs.redhome.cc/viewthread.php?tid=343395&extra=page%3D1",   NULL,   SW_SHOWMAXIMIZED   );
}

void CQZJ_NetKeeper_CrackerDlg::On32773()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CQZJ_NetKeeper_CrackerDlg::On32774()
{
	OnBnClickedCancel();
}

void CQZJ_NetKeeper_CrackerDlg::On32772()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://www.nov30th.com",   NULL,   SW_SHOWMAXIMIZED   );
}

void CQZJ_NetKeeper_CrackerDlg::OnUpdate()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://hoho.bz/Update/NetKeeper.Msi",   NULL,   SW_HIDE   );
}

void CQZJ_NetKeeper_CrackerDlg::On32778()
{
	ShellExecute(NULL,   "OPEN",   "HOHORootCA.cer", NULL,   NULL,   SW_NORMAL   );
}

void CQZJ_NetKeeper_CrackerDlg::On32777()
{
	ShellExecute(NULL,   "OPEN",   "��Ѷ֤��.cer", NULL,   NULL,   SW_NORMAL   );
}
void CQZJ_NetKeeper_CrackerDlg::OnBnClickedCommand1()
{
	OnBnClickedButtonlogin();
}

void CQZJ_NetKeeper_CrackerDlg::On32798()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://www.pediy.com",   NULL,   SW_SHOWMAXIMIZED   );
}

void CQZJ_NetKeeper_CrackerDlg::SetAutoRun(bool isactive) 
{
	HKEY hKey;
	LPCTSTR title = "HOHONetKeeper";
	CString m_filename;
	if (isactive==true)
		m_filename = GetCommandLine();
	else
		m_filename = ";";

	if (m_filename.Find("-autorun") <0)
	{
		m_filename += " -autorun";
	}
	LPCTSTR data_Set="Software\\Microsoft\\Windows\\CurrentVersion\\Run";//����ע�������ص�·��
	long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_WRITE,&hKey));//��ע����е���Ӧ��
	if(ret0!=ERROR_SUCCESS)	
	{
		MessageBox("����0x0000");
	}
	int i=0;
	int length = m_filename.GetLength()+1;//���ؼ��е����ݽ���ת�����Դﵽע����޸ĺ����Ĳ�����������
	DWORD cbData=length;
	LPBYTE lpb=new BYTE[length];
	int j=0;
	for(i=0;i<length;i++)
	{
		lpb[j]=m_filename[i];
		j++;
	}

	lpb[j]=0;
	long ret1=(::RegSetValueEx(hKey,title,NULL,REG_SZ,lpb,cbData));//����ص���Ϣд��ע���	
	//long ret1=(::RegSetValueEx(hKey,title,NULL,REG_SZ,lpb,cbData));//����ص���Ϣд��ע���

	if(ret1!=ERROR_SUCCESS)//�ж�ϵͳ�����ע���Ƿ�ɹ���

	{

		MessageBox("����0x0001");

	}

	delete lpb;

	::RegCloseKey(hKey);//�ر�ע����е���Ӧ����

}
void CQZJ_NetKeeper_CrackerDlg::OnBnClickedCheckautorun()
{
	UpdateData(true);
	if (m_isRemember == false)
	{
		AfxMessageBox("����ס����������,���޷��Զ���½.");
	}
	SetAutoRun(m_isAutoRun);
}


UINT CQZJ_NetKeeper_CrackerDlg::DownloadPatch(LPVOID pParam)
{
	VIRTUALIZER_START
		DeleteFile(patchFileName);
	CQZJ_NetKeeper_CrackerDlg* app = (CQZJ_NetKeeper_CrackerDlg*)pParam;

	DWORD dwAccessType = INTERNET_OPEN_TYPE_DIRECT;  // We are connecting to internet directly
	DWORD dwServiceType;
	CInternetSession session(("Nov30th"),0, dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	CString strServerName ;
	CString strObject;
	INTERNET_PORT nPort;
	//InternetSetOption (

	CString bbspath = patchAddress;
	AfxParseURL(bbspath, dwServiceType, strServerName, strObject, nPort);// ||   dwServiceType != AFX_INET_SERVICE_HTTPS);
	pServer = session.GetHttpConnection(strServerName, nPort);
	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL,INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE);
	try
	{
		pFile->SendRequest();
	}
	catch(...)
	{
		AfxMessageBox("�޷�����°汾��װ������");
		return 1;
	}
	CStdioFile f; //����ļ�����

	if( !f.Open( //������ļ�

		patchFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) ) 

	{ 
		AfxMessageBox("�޷�д���ļ�,�����������°汾."); 
		return 1;
	}
	//���潫����������浽�ļ���
	TCHAR szBuf[1024]; //����
	int length=0;
	long a=pFile->GetLength();
	while (length=pFile->Read(szBuf, 1023))
		f.Write(szBuf,length);
	pFile->Close();

	if (AfxMessageBox("�°汾�������,����ִ�а�װ?(�����жϵ�ǰ����.)",(MB_YESNO   |   MB_ICONQUESTION)   + 4096)==IDYES)
	{
		ShellExecute(NULL,   "OPEN",   patchFileName, NULL,   NULL,   SW_NORMAL   );
		isRun = false;
		app->OnBnClickedCancel();
	}
	VIRTUALIZER_END
		return 0;
}
void CQZJ_NetKeeper_CrackerDlg::On32799()
{
	OnBnClickedCancel();
}

void CQZJ_NetKeeper_CrackerDlg::On32801()
{
	On32771();
}

CString CQZJ_NetKeeper_CrackerDlg::HashString(CString yourString)
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

void CQZJ_NetKeeper_CrackerDlg::On32803()
{
	ShellExecute(NULL,   "OPEN",   "IEXPLORE.EXE",   "http://hoho.bz",   NULL,   SW_SHOWMAXIMIZED   );
}