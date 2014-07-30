#include "winbase.h"
#pragma once

#define	SOUP_MESSAGE_SIZE	1024

#define SIZE_OF_SYMBOL   8+1
#define SIZE_OF_MM       4+1

 
#define	SIZE_OF_CLT_NAME		30 + 1
#define	SIZE_OF_CLT_PASSWORD	30 + 1

#define	ERR_NONE			0
#define	ERR_INIT			1
#define	ERR_SOCKET_LISTEN	2
#define	ERR_SOCKET_PORT		3
#define	ERR_SOCKET_BINDING	4
#define	ERR_INI_WSA			5
#define	ERR_LISTENING		6
#define	ERR_ACCEPT			7
#define	ERR_MAX_CON_REACHED	8	
#define	ERR_START_LISTEN_THREAD		9
#define	ERR_CREATE_LISTEN_SOCKET	10
#define	ERR_CONNECT_SOCKET	11
#define	ERR_INVALID_SOCKET_DESCRIPTOR	12

#define	ERR_CONNECT					11
#define	ERR_LOGIN					12
#define	ERR_LOADING_OPTIONS					13

#define	 LOGIN_ACCEPTED				14
#define	 LOGIN_REJECTED_NOT_AUTH             15
#define	 LOGIN_REJECTED_SESSION_NOT_AVAIL             16



typedef struct SCommonStats  // Per each stock, price updates i.e. frequency of change
{
	ULONG	ulUpdatesPerSec;
	ULONG	ulMaxUpdatesPerSec;
	ULONG   ulAverageUpdatesPerSec; 	//ULONG   mini updates per sec will always revert to zero
	ULONG   ulTotalUpdates;
	ULONG   ulTotalBidVolume;
	ULONG   ulTotalAskVolume;
	ULONG   ulTotalBidOrders;
	ULONG   ulTotalAskOrders;

	ULONG   ulTotalBidPriceLevels;
	ULONG   ulTotalAskPriceLevels;

}SCOMMONSTATS;

typedef struct SMMDepth   // per each Individusal MM Entry in the price level
{
//	char		strSymbol[SIZE_OF_SYMBOL];
	char		strMM[SIZE_OF_MM];
	double		dPrice;	// each level will have one of this struct

	SCOMMONSTATS	SDepthStat;
	SYSTEMTIME		tLastUpdate;
	
	ULONG		ulAddMsg;
	ULONG		ulAddQty;
	ULONG		dAddQtyPerSec;

	ULONG		ulModifyMsg;
	ULONG		ulModifyQty;
	ULONG		ulModifyQtyPerSec;

	ULONG		ulCancelMsg;
	ULONG		ulCancelQty;
	ULONG		ulCancelQtyPerSec;

	ULONG		ulReplaceMsg;
	ULONG		ulReplaceQty;
	ULONG		ulReplaceQtyPerSec;

	ULONG		ulDeleteMsg;
	ULONG		ulDeleteQty;
	ULONG		ulDeleteQtyPerSec;

	ULONG		ulTradeNonCrossMsg;
	ULONG		ulTradeNonCrossQty;
	ULONG		ulTradeNonCrossQtyPerSec;

	ULONG		ulTotalMsg;
	ULONG		ulTotalQty;
	ULONG       ulTotalQtyPerSec;

	ULONG		ulTotalOpenOrder;  
	ULONG		ulTotalOpenOrderQty; // This is the one to be displayed for each MM
	ULONG		ulTotalOpenOrderPerSec;

	ULONG		ulTotalExecutedOrders;
	ULONG		ulTotalExecutedOrdersQty;
	ULONG		ulTotalExecutedOrdersPerSec;

	SMMDepth*	pNext;	// Next MM in the list

}SMMDEPTH; // per each Individusal MM Entry in the price level





typedef struct FeedMessageStats
{
	// Messages

	char			strStartTime[26];
	char			strEndTime[26];

}FEED_MESSAGE_STATS;

typedef struct Pic_Options
{
	char	szUserName[SIZE_OF_CLT_NAME];
	char	szPassword[SIZE_OF_CLT_PASSWORD];
	DWORD	dwIPAddress;
	UINT	uiPort;

	DWORD	dwIPAddress1;
	UINT	uiPort1;
	DWORD	dwBufferSize;
}PIC_CLT_OPTIONS;

	//PIC_CLT_OPTIONS setupinfo;

#define		SIZE_OF_TIME		32

typedef struct
{
	char	szIP[17];
	UINT	uiPortNumber;
	char	ConnectionTime[SIZE_OF_TIME/2];
	ULONG	ulNumMessages;
	ULONG	ulTransferedBytes;
}INBOUND_DATA;

typedef struct   // _LOCALFILEDATA  contains sizes, path +....
{
bool		bDeleted;		//0 = Inactive  1 = Active 
DWORD		dwSYSTEM_EVENT_MESSAGE;//1
DWORD		dwSTOCK_DIRECTORY_MESSAGE;//2
DWORD		dwSTOCK_TRADING_ACTION_MESSAGE;//3
DWORD		dwREG_SHO_RESTRICTION_MESSAGE;//4
DWORD		dwMP_POSITION_MESSAGE;//5
DWORD		dwMWCBDLM_MESSAGE;//6
DWORD		dwMWCBDBM_MESSAGE;//7
DWORD		dwIPO_QUOTATION_PERIOD_UPDATE_MESSAGE;//8
DWORD		dwADD_ORDER_NO_MPID_MESSAGE;//9
DWORD		dwADD_ORDER_MPID_MESSAGE;//10
DWORD		dwORDER_EXECUTED_MESSAGE;//11
DWORD		dwORDER_EXECUTED_WITH_PRICE_MESSAGE;//12
DWORD		dwORDER_CANCEL_MESSAGE;//13
DWORD		dwORDER_DELETE_MESSAGE;//14
DWORD		dwORDER_REPLACE_MESSAGE;//15
DWORD		dwTRADE_NON_CROSS_MESSAGE;//16
DWORD		dwNOII_MESSAGE;//17
DWORD		dwRPPI_MESSAGE;//18
DWORD		dwAllFeed;//19
DWORD		dwAccounts;//20

char	szPath[MAX_PATH];

}LOCALFILEDATA;









