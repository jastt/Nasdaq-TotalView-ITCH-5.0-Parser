#pragma once

#define	NASDAQ_GLOBAL_SELECT_MARKET		Q		//	NASDAQ Listed
#define	NASDAQ_GLOBAL_MARKET			G		//	NASDAQ Listed
#define	NASDAQ_CAPITAL_MARKET			S		//	NASDAQ Listed
#define	NYSE							N		//	Non-NASDAQ Listed
#define	NYSE_MKT						A		//	Non-NASDAQ Listed
#define	NYSE_ARCA						P		//	Non-NASDAQ Listed
#define	BATS							Z		//	Non-NASDAQ Listed
#define	SPACE							0X20	//	No Data


#define		ERR_CLEAR			 0
#define		INVALID_FILE_HANDLE 10
#define		ERR_FILE_MAPPING	20
#define		ERR_MAP_VIEW_OF_FILE	30
//Queue errors...CAN RETURN ERR_CLEAR
#define		ERR_ALLOCATION		10
#define		ERR_INIT_CRITICAL_SECTION 20
#define		ERR_QUEUE_ALLOCATION	40
#define		ERR_NOT_CONSTRUCTED		50


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
};

#undef MainModule
#else
extern char *SYSTEM_EVENT_CODES [SYSTEM_EVENT_CODES_SIZE][SYSTEM_EVENT_CODES_SIZE];
#endif
*/
#include "ItchIncludes.h"
#include <Sys/Timeb.h>

#define NAME_LENGTH	256
#define	SIZE_OF_KEY				39

typedef struct
{
	struct _timeb tRecv;
	struct _timeb tInBookServer;
	struct _timeb tRecvClient;
	struct _timeb tRecvInBookClient;
}MSG_TIME;

typedef struct   
{								//Name	Offset	Length	Value	Notes
	char				cMessageType;		//Message  Type	0	1	“S”	System Event Message.
	unsigned  int		iSockLocale;		//Stock Locate	1	2	Integer	Always 0
	unsigned  int		iTrackingNumber;	//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64		iTimeStamp;			//Timestamp	5	6	Integer	Nanoseconds since midnight.
	char				cEventCode;			//Event Code	11	1	Alpha	See System Event Codes below.
}SYSTEM_EVENT_MESSAGE;

//Financial Indicator  "EFSI" Struct member below

//D                   Deficient
//E                   Delinquent
//Q                  Bankrupt
//S                   Suspended
//G                  Deficient and Bankrupt
//H                   Deficient and Delinquent 
//J                   Delinquent and Bankrupt
//K                   Deficient, Delinquent and Bankrupt
//C	Creations and/or Redemptions Suspended for Exchange Traded Product
//N                   Normal (Default): Issuer Is NOT Deficient, Delinquent, or Bankrupt
//Non-NASDAQ-Listed Instruments
//<space>       Not available. Firms should refer to SIAC feeds for code if needed.


typedef struct  
{								//Name	Offset	Length	Value	Notes
	char	cMessageType;		//Message Type	0	1	“R”	Stock Directory Message
	unsigned int		iSockLocale;		// Stock Locate	1	2	Integer	Locate Code uniquely assigned to the security symbol for the day.
	unsigned int		iTracking_Number;	//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;			//Timestamp	5	6	Integer	Time at which the directory message was generated. Refer to Data Types for field processing notes.
 	char    strStock[SIZE_OF_SYMBOL]; //STOCK	11	8	Alpha	Denotes the security symbol for the issue in the NASDAQ execution system.
	unsigned int     eMarketCategory;				//Market Category	19	1	Alpha	Indicates Listing market or listing market tier for the issue
    unsigned int     eFSI;											//	Financial Status Indicator	20	1	Alpha
	unsigned int		iRoundLotSize;     //Round Lot Size	21	4	Integer	Denotes the number of shares that represent a round lot for the issue
	char	cRoundLotsOnly;		//Round Lots Only	25	1	Alpha  .... Y	NASDAQ OMX system only accepts round lots orders for this security. N N	NASDAQ OMX system does not have any order size restrictions for this security. Odd and mixed lot orders are allowed.
    char	cIssueClassification;	//Issue Classification	26	1	Alpha
	char	strIssueSubType[3];		// Issue Sub- Type	27	2	Alpha
	char	cAuthenticity;			// Authenticity	29	1	Alpha	29	1	  SHOW ONLY "P"       "T" for  Test
	char    cShortSaleThresholdInd;	//  Y N	or 0x20	//Short Sale Threshold Indicator 30	1                Alpha               Indicates if a security is subject to mandatory close-out of short sales under SEC Rule 203(b)(3).
	char	cIPOFlag;				// Y   N   0x20   IPO release.  This field is intended to help NASDAQ market participant firms comply with FINRA Rule 5131(b).
    char    cLULDRefPrice;			//  1	Tier 1 NMS Stocks and select ETPs 2 Tier 2 NMS Stocks    0x20
	char	cETPFlag;				// ETP Flag           33  1      Alpha    Y  Instrument is an ETP N Instrument is not an ETP
    unsigned int		iETPLeverageFactor;		//   34	4	Integer     	
	char    cInverseFactor;			// Inverse Indicator	38	1	Alpha  ... Y  ETP is an Inverse ETP  N ETP is not an Inverse ETP
}STOCK_DIRECTORY_MESSAGE;

typedef struct   
{
												//Name	Offset	Length	Value	Notes
	char	cMessageType;						//Message Type	0	1	“H”	Stock Trading Action Message.
	unsigned int		iStockLocate;			//	Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		iTrackingNumber;		//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;				// Timestamp	5	6	Integer	Nanoseconds since midnight
	char	Symbol[SIZE_OF_SYMBOL];				// Stock	11	8	Alpha	Stock symbol, right padded with spaces
	char	cTradingState;						// Trading State	19	1	Alpha	Indicates the current trading state for the stock. Allowable values:
	//'H' = Halted across all U.S. equity markets / SROs
	//'P' = Paused across all U.S. equity markets / SROs (NASDAQ-listed securities only)
	//'Q' = Quotation only period for cross-SRO halt or pause
	//'T' = Trading on NASDAQ
	char	cReserved;							//Reserved	20	1	Alpha	Reserved.
	char    strReason[5];						//Reason	21	4	Alpha	Trading Action reason.
}STOCK_TRADING_ACTION_MESSAGE;


typedef struct   
{//		Name	Offset	Length	Value	Notes
	char	cMessage;				//Message Type	0	1	“Y”	Reg SHO Short Sale Price Test Restricted Indicator
	unsigned int		iLocateCode;			// Locate Code	1	2	Integer	Locate code identifying the security 
	unsigned int		TrackingNumber;			// Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64		iTimeStamp;  // 	5	6	Integer	Nanoseconds since midnight
	char    Symbol[SIZE_OF_SYMBOL];    //  Stock	11	8	Alpha	Stock symbol, right padded with spaces
	char	cRegSHOAction;	// Reg SHO Action   19	1	Alpha	Denotes the Reg SHO Short Sale Price Test Restriction status for the issue at the time of the message dissemination.  Allowable values are:
	//“0” = No price test in place
	//“1” = Reg SHO Short Sale Price Test Restriction in effect due to an intra-day price drop in security
	//“2” = Reg SHO Short Sale Price Test Restriction remains in effect
}REG_SHO_RESTRICTION_MESSAGE;

typedef struct   
{//Name	Offset	Length	Value	Notes
	char				cMessageType;				//Message Type	0	1	“L”	Market Participant Position message
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Locate code identifying the security Tracking
	unsigned int		TrackingNumber;				//Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					// iTimeStamp	5	6	Integer	Nanoseconds since midnight
	char				strMPID[5];					// MPID	11	4	Alpha	Denotes the market participant identifier for which the position message is being generated
	char				Stock[SIZE_OF_SYMBOL];		//Stock 15	8	Alpha	Stock symbol, right padded with spaces
	char				cPrimaryMM;					// Y N		//Primary Market Maker	23	1	Alpha	Indicates if the market participant firm qualifies as a Primary Market Maker in accordance with NASDAQ marketplace rules
	char				cMMMode;					//Market Maker Mode	24	1	Alpha	Indicates the quoting participant’s registration status in relation to SEC Rules 101 and 104 of Regulation M 
										/*“N” = normal 
										“P” = passive “S” = syndicate
										“R” = pre-syndicate
										“L” = penalty*/
	char				cMarketParticipantState;		// Market Participant State	25	1	Alpha	Indicates the market participant’s current registration status in the issue 
										/*“A” = Active
										“E” = Excused/Withdrawn
										“W” = Withdrawn 
										“S” = Suspended
										“D” = Deleted*/
}MP_POSITION_MESSAGE;

typedef struct     
{ //Name	Offset	Length	Value	Notes
	char				cMessageType;				//Message Type	0	1	“V”	Market wide circuit breaker Decline Level Message.
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Always set to 0
	unsigned int		TrackingNumber;				//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					//Timestamp	5	6	Integer	Time at which the MWCB Decline Level message was generated
	double				dLevel1;					//Level 1	11	8	Price (8)	Denotes the MWCB Level 1 Value.
	double				dLevel2;					//Level 2	19	8	Price (8)	Denotes the MWCB Level 2 Value.
	double				dLevel3;					//Level 3	27	8	Price (8)	Denotes the MWCB Level 3 Value.
}MWCBDLM_MESSAGE;

typedef struct   
{ //Name	Offset	Length	Value	Notes
	char				cMessageType;				//Message Type	0	1	“W”	Market wide circuit breaker Decline Level Message.
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Always set to 0
	unsigned int		TrackingNumber;				//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					//Timestamp	5	6	Integer	Time at which the MWCB Decline Level message was generated
	char				cBreachLevel;					//Denotes the MWCB Level that was breached “1” = Level 1 “2” = Level 2
}MWCBDBM_MESSAGE;

typedef struct   
{ //Name	Offset	Length	Value	Notes
	char				cMessageType;	//Message Type	0	1	“K”	IPO Quoting Period Update Message
	unsigned int		iLocateCode;	//Stock Locate	1	2	Integer	Always set to 0
	unsigned int		TrackingNumber;	//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;		//Timestamp	5	6	Integer	Time at which the IPQ Quoting Period Update message was generated
	char				Stock[SIZE_OF_SYMBOL];	//Stock	11	8	Alpha	Denotes the security symbol for the issue in the NASDAQ execution system.
							
	unsigned int		iIPOQuotationReleaseTime; //IPO Quotation Release Time	19	4	Integer	Denotes the IPO release time, in seconds since midnight, for quotation to the nearest second.
//NOTE: If the quotation period is being canceled/postponed, we should state that
//1.   IPO Quotation Time will be set to 0
//2.   IPO Price will be set to 0
	 
	char				cIPOQuotationReleaseQualifier;	//IPO Quotation Release Qualifier	23	1	Alpha		Code	Description	
														// A	Anticipated quotation release time This value would be used when NASDAQ Market Operations initially enters the IPO instrument for release.	
														// C	IPO release canceled/postponed This value would be used when NASDAQ Market Operations cancels or postpones the release of the IPO instrument.	
	double				dIPOPrice;						//	24	4	Price (4)	Denotes the IPO price to be used for intraday net change calculations.
					// Prices are given in decimal format with 6 whole number places followed by 4 decimal digits. The whole number portion is padded on the left with spaces; the decimal portion is padded on the right with zeros. The decimal point is implied by position; it does not appear inside the price field.
}IPO_QUOTATION_PERIOD_UPDATE_MESSAGE;


typedef struct      // No MPID
{  // Name	Offset	Length	Value	Notes
	char				cMessageType;			//Message Type	0	1	“A”	Add Order – No MPID Attribution Message.
	unsigned int		iLocateCode;			//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;			//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;				//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;		//Order Reference Number	11	8	Integer	The unique reference number assigned to the new order at the time of receipt.
	char				cBuySell;				//Buy/Sell Indicator	19	1	Alpha	The type of order being added.
												//“B” = buy order. “S” = sell order.
	unsigned int		iShares;				//Shares	20	4	Integer	The total number of shares associated with the order being added to the book.
	char    Stock[SIZE_OF_SYMBOL];				//Stock	24	8	Alpha	Stock symbol, right padded with spaces
	double  dPrice;								//Price	32	4  Price (4)	The display price of the new order.
												//Refer to Data Types for field processing notes.
} ADD_ORDER_NO_MPID_MESSAGE;

typedef struct       //     ADD ORDER – MPID ATTRIBUTION MESSAGE
{	//Name	Offset	Length	Value	Notes
	char				cMessageType;		//Message Type	0	1	“F”	Add Order with MPID Attribution Message.
	unsigned int		iLocateCode;		//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;		//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;			//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;	//Order Reference Number	11	8	Integer	The unique reference number assigned to the new order at the time of receipt.
	char				cBuySell;  //Buy/Sell Indicator	19	1	Alpha	The type of order being added. “B” = buy order. “S” = sell order.
	unsigned int		iShares;//Shares	20	4	Integer	The total number of shares associated with the order being added to the book.
    char				Stock[SIZE_OF_SYMBOL]; //Stock	24	8	Alpha	Stock symbol, right padded with spaces
	double				dPrice;	 //Price	32	4	Price (4)	The display price of the new order.  //Refer to Data Types for field processing notes.
	char				strMPID[SIZE_OF_MM];			//Attribution	36	4	Alpha	NASDAQ market participant identifier associated with the entered order.
}ADD_ORDER_MPID_MESSAGE;

//4.4  Modify Order Messages
//Modify Order messages always include the Order Reference Number of the Add Order to which the update applies.
//To determine the current display shares for an order, ITCH 5.0 subscribers must deduct the number of shares stated in the Modify message 
//from the original number of shares stated in the Add Order message with the same reference number.
//NASDAQ may send multiple Modify Order messages for the same order reference number and the effects are cumulative. When the
//number of display shares for an order reaches zero, the order is dead and should be removed from the book.

//4.4.1 Order Executed Message
//This message is sent whenever an order on the book is executed in whole or in part. 
//It is possible to receive several Order Executed Messages for the same order reference number if that order is executed in several parts.
//The multiple Order Executed Messages on the same order are cumulative.
//
//By combining the executions from both types of Order Executed Messages and the Trade Message, 
//it is possible to build a complete view of all non-cross executions that happen on NASDAQ.
//Cross execution information is available in one bulk print per symbol via the Cross Trade Message.

typedef struct   
{// Name	Offset	Length	Value	Notes
	char				cMessageType;			//Message Type	0	1	“E”	Order Executed Message.
	unsigned int		iLocateCode;			//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;			//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;				//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;		//Order Reference Number	11	8	Integer	The order reference number associated with the executed order.
	unsigned int		iShares;				//Executed Shares	19	4	Integer	The number of shares executed.
	unsigned _int64     iOrderMatchNumber;		//Match Number	23	8	Integer	The NASDAQ generated day-unique Match Number of this execution. The match number is also referenced in the Trade Break Message.
} ORDER_EXECUTED_MESSAGE;


typedef struct   
{// Name	Offset	Length	Value	Notes
	char				cMessageType;			//Message Type	0	1	“c”	Order Executed Message.
	unsigned int		iLocateCode;			//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;			//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;				//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;		//Order Reference Number	11	8	Integer	The order reference number associated with the executed order.
	unsigned int		iShares;				//Executed Shares	19	4	Integer	The number of shares executed.
	unsigned _int64     iOrderMatchNumber;		//Match Number	23	8	Integer	The NASDAQ generated day-unique Match Number of this execution. The match number is also referenced in the Trade Break Message.
	
	char				cPrintable;				// Printable  31	1	Alpha	Indicates if the execution should be reflected on time and sale displays and volume calculations.
												//“N” = non-printable
												//“Y” = printable
	double				dExecutionPrice;		//Execution Price	32	4	Price (4)	The price at which the order execution occurred. Refer to Data Types for field processing notes.
} ORDER_EXECUTED_WITH_PRICE_MESSAGE;

typedef struct   
{ //Name	Offset	Length	Value	Notes
	char				cMessageType;				//Message Type	0	1	“X”	Order Cancel Message.
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;				//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;			//Order Reference Number	11	8	Integer	The reference number of the order being reduced.
	unsigned int		iShares;					//Canceled Shares	19	4	Integer	The number of shares being removed from the display size of the order as the result of a cancellation.
}ORDER_CANCEL_MESSAGE;


typedef struct   
{											//Name	Offset	Length	Value	Notes
	char				cMessageType;					//Message Type	0	1	“D”	Order Delete Message.
	unsigned int		iLocateCode;					//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;					//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;						//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;				//Order Reference Number	11	8	Integer	The reference number of the order being canceled.
}ORDER_DELETE_MESSAGE;

//This message is sent whenever an order on the book has been cancel-replaced. 
//All remaining shares from the original order are no longer accessible, and must be removed. 
//The new order details are provided for the replacement, along with a new order reference number which will be used henceforth. 
//Since the side, stock symbol and attribution (if any)cannot be changed by an Order Replace event, these fields are not included in the message.
//Firms should retain the side, stock symbol and MPID from the original Add Order message.

// :: Amro 7/14/2014  i.e. I should delete the Old order and "Add" a new order with the new Order Reference Number

typedef struct   
{//   Name	Offset	Length	Value	Notes
	char				cMessageType;						//Message Type	0	1	“U”	Order Replace Message
	unsigned int		iLocateCode;						//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;						//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;							//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOldOrderRefNumber;					//Original Order Reference Number	11	8	Integer	The original reference number of the order being replaced.
	unsigned _int64		iNewOrderRefNumber;					//New Order Reference Number	19	8	Integer	The new reference number for this order at time of replacement.
															//Please note that the NASDAQ system will use this new order reference number for all subsequent updates.
	unsigned int		iShares;							//Shares	27	4	Integer	The new total displayed quantity.
	double				dPrice;								//Price	31	4	Price (4)	The new display price for the order. Refer to Data Types for field processing notes.
}ORDER_REPLACE_MESSAGE;

typedef struct       //    Common Order Record Struct...Wil handle the order related issues...Add, delete, Modify
					// Does not get the data directly from a parsed message in the feed
{	
	char				cMessageType;			// Message Type		 	
												//“A”	Add Order – No MPID Attribution Message.
												//“F”	Add Order with MPID Attribution Message.
												//“E”	Order Executed Message.
												//“c”	Order Executed Message with price
												//“X”	Order Cancel Message.
												//“D”	Order Delete Message.
												//“U”	Order Replace Message

	unsigned int		iLocateCode;			// Stock Locate	Integer	Locate code identifying the security
//	unsigned int		TrackingNumber;			// Tracking Number		Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;				// Timestamp	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;		// Order Reference Number	Integer	The unique reference number assigned to the new order at the time of receipt.
	unsigned _int64		iPrevOrderRefNumber;	// Previous Order Reference Number	In case we are processing an order replace
	char				cBuySell;				// Buy/Sell Indicator			Alpha	The type of order being added. 
												// “B” = buy order.
												// “S” = sell order.
	unsigned int		iShares;				// Shares		Integer	The total number of shares associated with the order being added to the book.
    char				Stock[SIZE_OF_SYMBOL];	// Stock		8	Alpha	Stock symbol, right padded with spaces
	double				dPrice;					// Price		4	Price (4)	The display price of the new order.  //Refer to Data Types for field processing notes.
	char				strMPID[SIZE_OF_MM];	// Attribution	4	Alpha	NASDAQ market participant identifier associated with the entered order.
}COMMON_ORDER_MESSAGE;

//4.5.1 Trade Message (Non-Cross)
//The Trade Message is designed to provide execution details for normal match events involving non-displayable order types.
//(Note: There is a separate message for NASDAQ cross events.)
//Since no Add Order Message is generated when a non-displayed order is initially received, NASDAQ cannot use the Order Executed messages for all matches. 
//Therefore this message indicates when a match occurs between non-displayable order types. A Trade Message is transmitted each time a non-displayable 
//order is executed in whole or in part. It is possible to receive 
//multiple Trade Messages for the same order if that order is executed in several parts. Trade Messages for the same order are cumulative.
//Trade Messages should be included in NASDAQ time-and-sales displays as well as volume and other market statistics. Since Trade Messages do not affect the book, however, they may be ignored by firms just looking to build and track the NASDAQ execution system display.

typedef struct     
 {													//Name	Offset	Length	Value	Notes
	char				cMessageType;				//Message Type	0	1	“P”	Trade Message
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;				//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					//Timestamp	5	6	Integer	Nanoseconds since midnight.
	unsigned _int64		iOrderRefNumber;			//Order Reference Number	11	8	Integer	The unique reference number assigned to the order on the book being executed.
													//Effective December 6, 2010, NASDAQ OMX will populate the Order Reference Number field within the Trade (Non-Cross) message as zero. For the binary versions of the TotalView-ITCH data feeds, the field will be null-filled bytes (which encodes sequence of zero).
	char				cBuySell;					//Buy/Sell Indicator	19	1	Alpha	The type of non-display order on the book being matched.
													//“B” =buy order
													//“S” =sell order
	unsigned int		iShares;					//Shares	20	4	Integer	The number of shares being matched in this execution.
	char				Stock[SIZE_OF_SYMBOL];		//Stock	24	8	Alpha	Stock symbol, right padded with spaces
	double				dPrice;						//Price	32	4	Price (4)	The match price of the order.  Refer to Data Types for field processing notes.
	unsigned _int64		iMatchNumber;				//Match Number	36	8	Integer	The NASDAQ generated session-unique Match Number for this trade. The Match Number is referenced in the Trade Break Message.
}TRADE_NON_CROSS_MESSAGE;

//4.5.3 Broken Trade / Order Execution Message
//The Broken Trade Message is sent whenever an execution on NASDAQ is broken. An execution may be broken if it is found to be “clearly erroneous” pursuant to  NASDAQ’s Clearly Erroneous Policy.
//A trade break is final; once a trade is broken, it cannot be reinstated.
//Firms that use the ITCH feed to create time-and-sales displays or calculate market statistics should be prepared to process the broken trade message.  
//If a firm is only using the ITCH feed to build a book, however, it may ignore these messages as they have NO impact on the current book.

//BROKEN TRADE MESSAGE
//Name	Offset	Length	Value	Notes
//Message Type	0	1	“B”	Broken Trade Message.
//Stock Locate	1	2	Integer	Locate code identifying the security
//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
//Timestamp	5	6	Integer	Nanoseconds since midnight.
//Match Number	11	8	Integer	The NASDAQ Match Number of the execution that was broken. This refers to a Match Number from a previously transmitted Order Executed Message, Order Executed With Price Message,
//or Trade Message.
///////////////////////////////////////////////////////////////////////////////////

typedef struct    
{//Name	Offset	Length	Value	Notes
char    cMessageType;											//Message Type	0	1	“I”	NOII Message
unsigned int		iLocateCode;								//Stock Locate	1	2	Integer	Locate code identifying the security
unsigned int		TrackingNumber;								//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
unsigned _int64     iTimeStamp;									//Timestamp	5	6	Integer	Nanoseconds since midnight.
unsigned _int64		iPairedShares;								//Paired Shares	11	8	Integer	The total number of shares that are eligible to be matched at the Current Reference Price.
unsigned _int64     iImbalanceShares;							//Imbalance Shares	19	8	Integer	The number of shares not paired at the Current Reference Price.
unsigned int		iImbalanceDirection;						//Imbalance Direction	27	1	Alpha	The market side of the order imbalance.
																//“B” = buy imbalance “S” = sell imbalance “N” = no imbalance “O” = Insufficient orders to calculate
char    Stock[SIZE_OF_SYMBOL];									//Stock	28	8	Alpha	Stock symbol, right padded with spaces
double  dFarPrice;												//Far Price	36	4	Price (4)	A hypothetical auction-clearing price for cross orders only. Refer to Data Types for field processing notes.
double  dNearPrice;												//Near Price	40	4	Price (4)	A hypothetical auction-clearing price for cross orders as well as continuous orders. Refer to Data Types for field processing notes.
double  dRefPrice;												//Current Reference Price	44	4	Price (4)	The price at which the NOII shares are being calculated.  Refer to Data Types for field processing notes.
char    cCrossType;												//Cross Type	48	1	Alpha	The type of NASDAQ cross for which the NOII message is being generated
										//														//“O” = NASDAQ Opening Cross
										//														//“C” = NASDAQ Closing Cross
										//														//“H” = Cross for IPO and halted / paused securities
char	cPriceVariation;				//				Price Variation Indicator	49	1	Alpha	This field indicates the absolute value of the percentage of deviation of the Near Indicative Clearing Price to the nearest Current Reference Price.
										//															//“L” = Less than 1%
										//															//“1” = 1 to 1.99% 
										//															//“2” = 2 to 2.99% 
										//															//“3” = 3 to 3.99% 
										//															//“4” = 4 to 4.99% 
										//															//“5” = 5 to 5.99% 
										//															//“6” = 6 to 6.99% 
										//															//“7” = 7 to 7.99% 
										//															//“8” = 8 to 8.99% 
										//															//“9” = 9 to 9.99% 
										//															//“A” = 10 to 19.99% 
										//															//“B” = 20 to 29.99%
										//															//“C” = 30% or greater
										//															//Space = Cannot be calculated
}NOII_MESSAGE;


typedef struct    
{//Name	Offset	Length	Value	Notes
	char    cMessageType;							//Message Type	0	1	“N”	Retail Interest message
	unsigned int		iLocateCode;				//Stock Locate	1	2	Integer	Locate code identifying the security
	unsigned int		TrackingNumber;				//Tracking Number	3	2	Integer	NASDAQ OMX internal tracking number
	unsigned _int64     iTimeStamp;					//Timestamp	5	6	Integer	Nanoseconds since midnight.
	char    Stock[SIZE_OF_SYMBOL];					//Stock	11	8	Alpha	Stock symbol, right padded with spaces
	char    cInterestFlag;							//Interest Flag	19	1	Alpha	“B” = RPI orders available on the buy side
													//“S” = RPI orders available on the sell side
													//“A” = RPI orders available on both sides (buy and sell)
													//“N” = No RPI orders available
}RPPI_MESSAGE;

typedef struct  
{
	bool	bDeleted;
	char	szKey[SIZE_OF_KEY];			// store the key
	RPPI_MESSAGE RPPI;
	MSG_TIME tMessage; 
}RPPI_MESSAGE_RTDB;




#define	MAX_TABLES				36
#define MAX_MESSAGE_TYPES		23

#ifdef __cplusplus
extern "C"{
#endif

#ifdef MAIN_MODULE	
char* szTables [MAX_TABLES] = /*Index 0 to 17  CARVED IN STONE....DO NOT CHANGE ORDER*/{"SYSTEM_EVENT_MESSAGE_RTDB","STOCK_DIRECTORY_MESSAGE_RTDB", "STOCK_TRADING_ACTION_MESSAGE_RTDB", \
/*3-5*/								"REG_SHO_RESTRICTION_MESSAGE_RTDB", "MP_POSITION_MESSAGE_RTDB", "MWCBDLM_MESSAGE_RTDB",\
/*6-8*/								"MWCBDBM_MESSAGE_RTDB", "IPO_QUOTATION_PERIOD_UPDATE_MESSAGE_RTDB", "ADD_ORDER_NO_MPID_MESSAGE_RTDB",\
/*9-11*/							"ADD_ORDER_MPID_MESSAGE_RTDB", "ORDER_EXECUTED_MESSAGE_RTDB", "ORDER_EXECUTED_WITH_PRICE_MESSAGE_RTDB",\
/*12-14*/							"ORDER_CANCEL_MESSAGE_RTDB", "ORDER_DELETE_MESSAGE_RTDB", "ORDER_REPLACE_MESSAGE_RTDB",\
/*15-17*/							"TRADE_NON_CROSS_MESSAGE_RTDB", "NOII_MESSAGE_RTDB", "RPPI_MESSAGE_RTDB", \
/*18- 19*/							"ASSET_RTDB", "COMMON_ORDER_MESSAGE_RTDB"};

	char* szMessages [MAX_MESSAGE_TYPES] = {  // CARVED IN STONE....DO NOT CHANGE ORDER...WILL AFFECT the FillMsgStructs and the UI
	"SystemEvent",					//Index 0
	"Stock Directory",				//Index 1
	"Stock Trading Action",			//Index 2
	"Reg Sho Restriction",			//Index 3
	"Add Order",					//Index 4
	"Add Order With MPID",			//Index 5
	"Modify Order",					//Index 6
	"Order Executed",				//Index 7
	"Order Executed With Price",	//Index 8
	"Order Cancel",					//Index 9
	"Order Delete",					//Index 10
	"Order Replace",				//Index 11
	"Trade Non Cross",				//Index 12
	"Trade Cross",					//Index 13
	"Market Maker Position",		//Index 14
	"MWCB Decline Level",			//Index 15	//MarketWideCircuitBreaker"
	"MWCB Breach",					//Index 16
	"IPO Quoting Period",			//Index 17
	"Broken Trade",					//Index 18
	"NOI",							//Index 19
	"RPII",							//Index	20	// Retail price improvement indicator
	"Control",						//Index 21
	"Total Messages"				//Index 22
	};

#undef MAIN_MODULE
#else
extern char* szTables[MAX_TABLES];
extern char* szMessages [MAX_MESSAGE_TYPES];
#endif

#ifdef __cplusplus 
}
#endif
