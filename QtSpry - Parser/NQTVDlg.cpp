
// QtSpryDlg.cpp : implementation file
//

#include "stdafx.h"

#define MAIN_MODULE
#include "NQTV.h"
#include "NQTVDlg.h"
#include "Util.h"
#include "ITCHMessages.h"
#undef MAIN_MODULE
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "NQTVFeed.h"
 
HANDLE CNQTVDlg::m_FeedThreadHandle = 0;
HANDLE CNQTVDlg::m_ProcessThreadHandle = 0;

CNQTVFeed  *CNQTVDlg::m_pCNQTVFeed;
CProcessNQTVFeed  *CNQTVDlg::m_pCProcessFeed;

CReceiveITCH  *CNQTVDlg::m_pCQtReceiveITCH;

CListCtrl CNQTVDlg::m_ctrlListStats;
FEED_MESSAGE_STATS CNQTVDlg::m_MessageStats;
bool CNQTVDlg::m_bTestFeed = false;
bool CNQTVDlg::m_bReplay = false;
static unsigned long arrMessagesPerSec[MAX_MESSAGE_TYPES];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
///////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CNQTVDlg dialog
///////////////////////////////////////////////////////////////////////////////////
CNQTVDlg::CNQTVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNQTVDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTestFeed = false;
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATS, m_ctrlListStats);
	DDX_Control(pDX, IDC_BUTTON_FEEDSTART, m_ctrlButtonFeedStart);
	DDX_Control(pDX, IDC_BUTTON_FEED_STOP, m_ctrlButtonFeedStop);
	DDX_Control(pDX, IDC_BUTTON_FEED_PAUSE, m_ctrlButtonFeedPause);
	DDX_Control(pDX, IDC_BUTTON_PROCESS_START, m_ctrlButtonProcessStart);
	DDX_Control(pDX, IDC_BUTTON_PROCESS_STOP, m_ctrlButtonProcessStop);
	DDX_Control(pDX, IDC_BUTTON_PROCESS_PAUSE, m_ctrlButtonProcessPause);
	DDX_Control(pDX, IDC_BUTTON1, m_ctrlRunTestFeed);
}
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CNQTVDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FEED_PAUSE, &CNQTVDlg::OnBnClickedButtonFeedPause)
	ON_BN_CLICKED(IDC_BUTTON_FEED_STOP, &CNQTVDlg::OnBnClickedButtonFeedStop)
	ON_BN_CLICKED(IDC_BUTTON_FEEDSTART, &CNQTVDlg::OnBnClickedButtonFeedStart)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_PAUSE, &CNQTVDlg::OnBnClickedButtonProcessPause)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_STOP, &CNQTVDlg::OnBnClickedButtonProcessStop)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_START, &CNQTVDlg::OnBnClickedButtonProcessStart)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH_ALL, &CNQTVDlg::OnBnClickedButtonFlushAll)
	ON_BN_CLICKED(IDC_BUTTON_FLUSH_SINGLE, &CNQTVDlg::OnBnClickedButtonFlushSingle)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CNQTVDlg::OnToolsOptions)
	ON_COMMAND(ID_TOOLS_RTDDFILESIZE, &CNQTVDlg::OnToolsRtddfilesize)
	ON_BN_CLICKED(IDC_BUTTON1, &CNQTVDlg::OnBnClickedTestFeed)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////
// CNQTVDlg message handlers
BOOL CNQTVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

 	m_ctrlButtonFeedPause.EnableWindow(false);
	m_ctrlButtonFeedStop.EnableWindow(false);
	m_ctrlButtonProcessPause.EnableWindow(false);
    m_ctrlButtonProcessStop.EnableWindow(false);
	m_ctrlListStats.SetExtendedStyle( LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE|LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	// Columns
	m_ctrlListStats.InsertColumn(0, _T("Message Type") , LVCFMT_LEFT, 200);  
	m_ctrlListStats.InsertColumn(1, _T("Received Per Second") , LVCFMT_LEFT, 200);  
	m_ctrlListStats.InsertColumn(2, _T("Max Per Second"), LVCFMT_LEFT, 200);
	m_ctrlListStats.InsertColumn(3, _T("Total"), LVCFMT_LEFT, 200);
	// Items


	m_ctrlListStats.DeleteAllItems();

	for (int ii  = 0; ii < MAX_MESSAGE_TYPES; ii++)
	{
		if (!szMessages[ii])
			break;

		m_ctrlListStats.InsertItem(ii, _T(szMessages[ii]));	// 0  per second, 1 max per second,  2 total
	}
	
	m_ctrlListStats.InsertItem(MAX_MESSAGE_TYPES, _T("Start Time"));
	m_ctrlListStats.SetItem(MAX_MESSAGE_TYPES, 1, LVIF_TEXT, CString(theApp.g_Stats.strStartTime), 0,0,0,0);

	memset(theApp.g_arrMessagesPerSec, 0, sizeof(theApp.g_arrMessagesPerSec));
	memset(theApp.g_arrMaxMessagesPerSec, 0, sizeof(theApp.g_arrMaxMessagesPerSec));
	memset(theApp.g_arrTotalMessages, 0, sizeof(theApp.g_arrTotalMessages));

	return TRUE;  // return TRUE  unless you set the focus to a control
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
///////////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
///////////////////////////////////////////////////////////////////////////////////
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNQTVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonFeedStart()  // feed start
{  
		memset(theApp.g_arrMessagesPerSec,		0, sizeof(theApp.g_arrMessagesPerSec));
		memset(theApp.g_arrMaxMessagesPerSec,	0, sizeof(theApp.g_arrMaxMessagesPerSec));
		memset(theApp.g_arrTotalMessages,		0, sizeof(theApp.g_arrTotalMessages));

		ResetPerSec();
		
		CUtil	CUtil;
		
		for (int ii = 0; ii < MAX_MESSAGE_TYPES; ii++)
		{
			m_ctrlListStats.SetItem(ii, 1 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrMessagesPerSec[ii]) , 0, 0, 0, 0);
			m_ctrlListStats.SetItem(ii, 2 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrMaxMessagesPerSec[ii]) , 0, 0, 0, 0);
			m_ctrlListStats.SetItem(ii, 3 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrTotalMessages[ii]) , 0, 0, 0, 0);
		}

		unsigned int m_uiSpryThreadID = 0;		// :: Start the stat thread
		// ::log info
		theApp.g_iFlag = theApp.g_iFeedStatus = STARTING;

		m_bReplay = true;

		m_FeedThreadHandle = (HANDLE)_beginthreadex((LPSECURITY_ATTRIBUTES)NULL, 0, FeedThread, NULL, CREATE_SUSPENDED, &m_uiFeedThreadID);
		SetThreadPriority(m_FeedThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);  //  rush here
		
		ResumeThread(m_FeedThreadHandle);

		m_ctrlButtonFeedStart.EnableWindow(false);
		m_ctrlButtonFeedPause.EnableWindow(true);
	    m_ctrlButtonFeedStop.EnableWindow(true);

 		m_uTimer = SetTimer(1, 1000,  &TimerProc); // once per second
		if (m_uTimer == 0) 
		{ 
			//ErrorHandler("No timer is available."); 
		}

		theApp.g_iFlag = theApp.g_iFeedStatus = RUNNING;
		
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonFeedPause()  // Feed Pause
{
 	
	theApp.g_iFlag = theApp.g_iFeedStatus = PAUSSING;
	//init the pause flag
	
	m_ctrlButtonFeedStart.EnableWindow(true);
	m_ctrlButtonFeedPause.EnableWindow(false);
    m_ctrlButtonFeedStop.EnableWindow(true);
	
	theApp.g_iFlag = theApp.g_iFeedStatus = PAUSSED;

}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonFeedStop()  // Feed Stop
{

	theApp.g_iFlag = theApp.g_iFeedStatus = STOPPING;
	theApp.g_bReceiving = false;

	//theApp.g_bConnected = false;

	// terminate the thread
	m_ctrlButtonFeedStart.EnableWindow(true);
	m_ctrlButtonFeedPause.EnableWindow(false);
    m_ctrlButtonFeedStop.EnableWindow(false);

	//init the Stop flag
	//thread will terminate through completion

	theApp.g_iFlag = theApp.g_iFeedStatus = STOPPED;

	if (m_uTimer)
		KillTimer(m_uTimer);

	ResetPerSec();
//	SetMaxPerSec();

}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonProcessPause()  // Process Pause
{
	theApp.g_iFlag = theApp.g_iProcessMessageStatus = PAUSSING;

	m_ctrlButtonProcessStart.EnableWindow(true);
	m_ctrlButtonProcessPause.EnableWindow(false);
    m_ctrlButtonProcessStop.EnableWindow(true);

	theApp.g_iFlag = theApp.g_iProcessMessageStatus = PAUSSED;

}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonProcessStop()    //  Process Stop
{
	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STOPPING;

	m_ctrlButtonProcessStart.EnableWindow(true);
	m_ctrlButtonProcessPause.EnableWindow(false);
    m_ctrlButtonProcessStop.EnableWindow(false);


	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STOPPED;

}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonProcessStart()   // Process Start
{

	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STARTING;

	m_ctrlButtonProcessStart.EnableWindow(false);
	m_ctrlButtonProcessPause.EnableWindow(true);
    m_ctrlButtonProcessStop.EnableWindow(true);
	
	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STARTING;

	m_ProcessThreadHandle = (HANDLE)_beginthreadex((LPSECURITY_ATTRIBUTES)NULL, 0, ProcessFeedThread, NULL, CREATE_SUSPENDED, &m_uiProcessThreadID);
	SetThreadPriority(m_ProcessThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);  // no rush here!!!
	
	ResumeThread(m_ProcessThreadHandle);

	m_ctrlButtonFeedStart.EnableWindow(false);
	m_ctrlButtonFeedPause.EnableWindow(true);
    m_ctrlButtonFeedStop.EnableWindow(true);

 	theApp.g_iFlag = theApp.g_iProcessMessageStatus = RUNNING;

 
}
///////////////////////////////////////////////////////////////////////////////////
unsigned  __stdcall   CNQTVDlg::ProcessFeedThread(void*)   // Process feed thread
{
	m_pCProcessFeed = NULL;

	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STARTING;

	m_pCProcessFeed = new CProcessNQTVFeed();

	if (!m_pCProcessFeed)
	{
		//::log error instantiating Spryware
		theApp.g_iFlag = theApp.g_iProcessMessageStatus = PROCESSERROR;
		return -1; // TODO enum the error codes
	}
	//log info

	// Thread termination through completion
	delete (m_pCProcessFeed);
	m_pCProcessFeed = NULL;

	theApp.g_iFlag = theApp.g_iProcessMessageStatus = STOPPED;

	return 1;// TODO enum the error codes
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonFlushAll()
{
 
}
///////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedButtonFlushSingle()
{
 
}

///////////////////////////////////////////////////////////////////////////////////
unsigned  __stdcall   CNQTVDlg::FeedThread(void*)  // Incomming Feed  thread....
{

	theApp.g_iFlag = theApp.g_iFeedStatus = STARTING;
	CReceiveITCH*  m_pCQtReceiveITCH = NULL;

	//:: TODO get the last sequence number and pass to QTtCom
	if (m_bTestFeed)
	{
		SOCKET  SocketTV = INVALID_SOCKET;  // useless...just for compliance
 		m_pCQtReceiveITCH = new CReceiveITCH(&SocketTV );

		if (!m_pCQtReceiveITCH)
			return -1;
		//::Amro  Actual Grinding goes here
		theApp.g_bReceiving = true;

		char strFileName[MAX_PATH] = {"C:\\ITCH5.0TestFeed\\06022014.NASDAQ_ITCH50"};
		m_pCQtReceiveITCH->ReadFromTestFile(strFileName);   // this is the main loop
	}
	else
	{
		return -1;
	}

	//log info
 	// Thread termination through completion
	if (m_pCQtReceiveITCH)
	{
		delete m_pCQtReceiveITCH;
		m_pCQtReceiveITCH = NULL;
	}

	m_ctrlListStats.InsertItem(MAX_MESSAGE_TYPES + 1, _T("End Time"));
	m_ctrlListStats.SetItem(MAX_MESSAGE_TYPES+ 1, 1, LVIF_TEXT, CString(theApp.g_Stats.strEndTime), 0,0,0,0);

	return 1;// TODO enum the error codes
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK CNQTVDlg::TimerProc(HWND hwnd, UINT message, UINT_PTR idTimer, DWORD dwTime)
{ 
	CUtil	CUtil;

	SetPerSec();
	SetMaxPerSec();
 
	for (int ii = 0; ii < MAX_MESSAGE_TYPES; ii++)
	{
		m_ctrlListStats.SetItem(ii, 1 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrMessagesPerSec[ii]) , 0, 0, 0, 0);
		m_ctrlListStats.SetItem(ii, 2 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrMaxMessagesPerSec[ii]) , 0, 0, 0, 0);
		m_ctrlListStats.SetItem(ii, 3 , LVIF_TEXT,   CUtil.UnsingedToCString(theApp.g_arrTotalMessages[ii]) , 0, 0, 0, 0);
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::SetPerSec()
{
 	static unsigned long arrTotalMessages[MAX_MESSAGE_TYPES];

	if (m_bReplay)
	{
		memset(arrTotalMessages, 0, sizeof(arrTotalMessages)); 
		memset(theApp.g_arrMaxMessagesPerSec, 0, sizeof(theApp.g_arrMaxMessagesPerSec)); 

		m_bReplay = false;
	}

	for (int ii = 0; ii < MAX_MESSAGE_TYPES; ii++)
		theApp.g_arrMessagesPerSec[ii]   =  theApp.g_arrTotalMessages[ii]  -  arrTotalMessages[ii];

	memmove(arrTotalMessages, theApp.g_arrTotalMessages, sizeof(arrTotalMessages));   // Move total messages to local total

}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::ResetPerSec()
{
	memset(theApp.g_arrMessagesPerSec, 0, sizeof(theApp.g_arrMessagesPerSec));
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::SetMaxPerSec()
{

	for (int ii = 0; ii < MAX_MESSAGE_TYPES; ii++)
		theApp.g_arrMaxMessagesPerSec[ii]   = (theApp.g_arrMaxMessagesPerSec[ii] > arrMessagesPerSec[ii]) ? theApp.g_arrMaxMessagesPerSec[ii] : arrMessagesPerSec[ii];

	memmove(arrMessagesPerSec, theApp.g_arrMessagesPerSec, sizeof(arrMessagesPerSec)); 
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnToolsOptions()
{
	
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnToolsRtddfilesize()
{
	
}
//////////////////////////////////////////////////////////////////////////////////////////////
void CNQTVDlg::OnBnClickedTestFeed()
{
	m_bTestFeed = true;

	m_ctrlRunTestFeed.SetWindowTextA("Running In Replay Mode");

}
//////////////////////////////////////////////////////////////////////////////////////////////