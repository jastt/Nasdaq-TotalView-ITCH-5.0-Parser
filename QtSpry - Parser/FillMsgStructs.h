#pragma once

#include "Util.h"

#include "ITCHMessages.h"


class CFillMsgStructs
{
public:
	CFillMsgStructs(void);
	virtual ~CFillMsgStructs(void);

	int DirectToMethod(UINT8* strMsg);

	int	SystemEvent(UINT8* strMsg);
	int	StockDirectory(UINT8* strMsg);
	int StockTradingAction(UINT8* strMsg);
	int RegShoRestriction(UINT8* strMsg);
	int Market_Participant_Position(UINT8* strMsg);
	int MWCBDeclineLevelMessage(UINT8* uiMsg);
	int MWCBBreachMessage(UINT8* uiMsg);
	int IPOQuotingPeriodUpdate(UINT8* uiMsg);
	int AddOrderNoMPIDMessage(UINT8* uiMsg);
	int AddOrderWithMPID(UINT8* uiMsg);
	int OrderExecutionMessage(UINT8* uiMsg);
	int OrderExecutionWithPriceMessage(UINT8* uiMsg);
	int OrderCancelMessage(UINT8* uiMsg);
	int OrderDelete(UINT8* uiMsg);
	int OrderReplace(UINT8* uiMsg);
	int TradeMessageNonCross(UINT8* uiMsg);
	int NOII(UINT8* uiMsg);
	int RetailPriceImprovementIndicator(UINT8* uiMsg);

	FEED_MESSAGE_STATS GetStats();

private:

	CUtil										*m_pCUtil;
	SYSTEM_EVENT_MESSAGE						m_SystemEvent;
	STOCK_DIRECTORY_MESSAGE						m_StockDirectory;
	STOCK_TRADING_ACTION_MESSAGE				m_StockTradingAction;
	REG_SHO_RESTRICTION_MESSAGE					m_RegShoRestricted;
	MP_POSITION_MESSAGE							m_MPPosition;
	MWCBDLM_MESSAGE								m_MWCBDLM;
	MWCBDBM_MESSAGE								m_MWCBDBM;	
	IPO_QUOTATION_PERIOD_UPDATE_MESSAGE			m_IPOQuotationPeriod;
	ADD_ORDER_NO_MPID_MESSAGE					m_AddOrderNoMPID;
	ADD_ORDER_MPID_MESSAGE						m_AddOrderMPID;
	ORDER_EXECUTED_MESSAGE						m_OrderExecuted;
	ORDER_EXECUTED_WITH_PRICE_MESSAGE			m_OrderExecutedWithPrice;
	ORDER_CANCEL_MESSAGE						m_OrderCancel;
	ORDER_DELETE_MESSAGE						m_OrderDelete;
	ORDER_REPLACE_MESSAGE						m_OrderReplace;
	TRADE_NON_CROSS_MESSAGE						m_TradeNonCross;
	NOII_MESSAGE								m_NOII;
	RPPI_MESSAGE								m_RPPI;
};

//ITCH messages
// – DAILY
/*
#define  SYSTEM_EVENT_CODES_SIZE  7
#ifdef  MainModule
char *SYSTEM_EVENT_CODES [SYSTEM_EVENT_CODES_SIZE][SYSTEM_EVENT_CODES_SIZE] = {
	{"O", "Start of Messages. Outside of time stamp messages, the start of day message is the first message sent in any trading day"},
	{"S",  "Start of System hours. This message indicates that NASDAQ is open and ready to start accepting orders"},
	{"Q",  "Start of Market hours. This message is intended to indicate that Market Hours orders are available for execution"},
	{"M",  "End of Market hours. This message is intended to indicate that Market Hours orders are no longer available for execution"},
	{"E",   "End of System hours. It indicates that NASDAQ is now closed and will not accept any new orders today. It is still possible to receive Broken Trade messages and Order Delete messages after the End of Day"},
	{"C",	"End of Messages. This is always the last message sent in any trading day"}
*/
