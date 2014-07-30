
// QtSpryDlg.h : header file
//
#pragma once
#include "NQTVFeed.h"
#include "ReceiveITCH.h"
#include "ProcessNQTVFeed.h"
#include "afxcmn.h"
#include "afxwin.h"
 


// CNQTVDlg dialog
class CNQTVDlg : public CDialog
{
// Construction
public:
	CNQTVDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//enum { IDD = IDD_QTSPRY_DIALOG };
	enum { IDD = IDD_QT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static unsigned __stdcall ProcessFeedThread(void*);
	static unsigned __stdcall FeedThread(void*);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	 
	static CListCtrl m_ctrlListStats;

	//static unsigned long m_arrMessagesPerSec [MAX_MESSAGE_TYPES];
	//static unsigned long m_arrMaxMessagesPerSec [MAX_MESSAGE_TYPES];


	afx_msg void OnBnClickedButtonFeedPause();
	afx_msg void OnBnClickedButtonFeedStop();
	afx_msg void OnBnClickedButtonFeedStart();
	afx_msg void OnBnClickedButtonProcessPause();
	afx_msg void OnBnClickedButtonProcessStop();
	afx_msg void OnBnClickedButtonProcessStart();
	afx_msg void OnBnClickedButtonFlushAll();
	afx_msg void OnBnClickedButtonFlushSingle();

private:
	static void  CALLBACK  TimerProc( HWND hwnd, UINT message, UINT_PTR idTimer,  DWORD dwTime);

	UINT_PTR m_uTimer; 
	
	static	 CNQTVFeed  *m_pCNQTVFeed;

	static   CReceiveITCH*  m_pCQtReceiveITCH;
	static	CProcessNQTVFeed   *m_pCProcessFeed;

	static FEED_MESSAGE_STATS m_MessageStats;
	static HANDLE m_FeedThreadHandle;
	static HANDLE m_ProcessThreadHandle;

	static bool	m_bReplay;

	unsigned int m_uiFeedThreadID;
	unsigned int m_uiProcessThreadID;

	static void ResetPerSec();
	static void SetMaxPerSec();
	static void SetPerSec();

	static bool m_bTestFeed;

public:
	CButton m_ctrlButtonFeedStart;
	CButton m_ctrlButtonFeedStop;
	CButton m_ctrlButtonFeedPause;
	CButton m_ctrlButtonProcessStart;
	CButton m_ctrlButtonProcessStop;
	CButton m_ctrlButtonProcessPause;
	afx_msg void OnToolsOptions();
	afx_msg void OnToolsRtddfilesize();
	afx_msg void OnBnClickedTestFeed();
	CButton m_ctrlRunTestFeed;
};
