
// QtSpry.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "ITCHIncludes.h"
  
enum feedStatus
{
	OK = 0,
	STARTING,
	RUNNING,
	PAUSSING,
	PAUSSED,
	RESUMING,
	RESUMED,
	STOPPING,
	STOPPED,
	STARTCOMERROR,
	PROCESSERROR
};

// CSpryApp:
// See QtSpry.cpp for the implementation of this class
//

#include "ItchIncludes.h"

#define MAX_MESSAGE_TYPES    23

class CSpryApp : public CWinAppEx
{
public:
	CSpryApp();
	~CSpryApp();


 
	int		g_iFlag;
	int		g_iFeedStatus;
	int		g_iProcessMessageStatus;


	bool	g_bConnected;
	bool	g_bReceiving;

	FEED_MESSAGE_STATS g_Stats;
	LOCALFILEDATA  SMemoryMappedFileSSize;

	PIC_CLT_OPTIONS	m_SOptions;
	unsigned long g_arrTotalMessages [MAX_MESSAGE_TYPES];
	unsigned long g_arrMessagesPerSec [MAX_MESSAGE_TYPES];
	unsigned long g_arrMaxMessagesPerSec [MAX_MESSAGE_TYPES];

// Overrides
public:
	virtual BOOL InitInstance();

 // Implementation
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSpryApp theApp;