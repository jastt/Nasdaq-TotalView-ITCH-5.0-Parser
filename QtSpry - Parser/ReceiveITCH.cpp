#include "stdafx.h"
#include "afx.h"
#include "fcntl.h"
#include "Share.h"
#include "Io.h"
#include "ReceiveITCH.h"
#include "NQTV.h"


CReceiveITCH::CReceiveITCH(SOCKET *Sock): m_Socket(*Sock)
{
	int		iRet = 0;
	bool 
	bInitError = false;
	m_iLen = 0;

	m_pCUtil = NULL;
	m_pCUtil = new CUtil();


	m_iOutputBuffLen	= (theApp.m_SOptions.dwBufferSize) * COMP_FACTOR;		// Compression factor for inflating the receive buffer
	m_iTVBufLen		=   theApp.m_SOptions.dwBufferSize;   // in case we go overboard by stopping at the edge of a new message;
	m_pszRecvBufLen =   theApp.m_SOptions.dwBufferSize;


	m_pszTVBuf		= NULL;
	m_pszRecvBuf	= NULL;
	m_pszOutputBuf	= NULL;
 
	m_pszRecvBuf		= new   UINT8 [ m_pszRecvBufLen +1 ]; // Data received from Socket
	m_pszOutputBuf		= new   UINT8 [ m_iOutputBuffLen + 1];
	m_pszTVBuf			= new   UINT8 [ m_iTVBufLen + 1000];        // guard for extra...

	if ((m_pszTVBuf == NULL) || (m_pszRecvBuf == NULL) || (m_pszOutputBuf == NULL))
	{
		bInitError = true;
		// log error 
		return;
	}

	m_pFillStructs = NULL;
	m_pFillStructs = new CFillMsgStructs();
	if (!m_pFillStructs)
	{
		// log error 
		return;
	}

	m_pLast = NULL;
	m_pBegin = NULL;
}
#define BUFFER_EXTRA    300
/////////////////////////////////////////////////////////////////////////////////////////
int  CReceiveITCH::ProcessFeed()
{
	int	i					= 0;
	__int64	lReceiveTime	= 0;
	int		iMessageLength	= 0;

	time(&lReceiveTime);
	int	ii = 0;

	m_pBegin = &m_pszTVBuf[0];
	m_pLast = m_pBegin;

	int iTerationNum = 0;
	int iPrevMsgLen = 0;

	_int64  ulTotalBytesToBeProcessed = 0;
	_int64  ulBytesLeft = BUFFER_EXTRA;
 
 	while (m_pBegin )
	{
		iTerationNum++;
		memset(m_strPacketLength, '\0', 3);
 		memmove(m_strPacketLength, m_pBegin, 2);
		m_iPacketLength =  m_pCUtil->GetValueShort(m_strPacketLength, 0, 2); 

		ulTotalBytesToBeProcessed += (iPrevMsgLen + 2);  //  total to "BE" processed
		ulBytesLeft = m_iLen - ulTotalBytesToBeProcessed ;   // bytes left AFTER processing this one

		if (ulBytesLeft < BUFFER_EXTRA)   // buffer for extra large message
		{
			ulBytesLeft += 2;
 			break; 
		}
		
		memset(m_strPacketType, '\0', 2);
		memmove(m_strPacketType, m_pBegin + 2, 1);

		m_iPacketType = m_pCUtil->GetValueChar(m_strPacketType, 0, 1);

		if (m_iPacketLength)  // :: TODO guard for extensive length 
		{
			memset(m_szMessage, '\0', sizeof(m_szMessage));  // reset with every EOM
			memmove (m_szMessage, m_pBegin + 2, m_iPacketLength);

			//switch (m_iPacketType)
			//{
			//case 'S': //SequencedMsg();
				m_pFillStructs->DirectToMethod(m_szMessage);
			//			break;
		//	case 'H': //ServerHB
			//			break;
		//	case 'Z': //End of Session
		//			theApp.g_bReceiving = false;
		//				break;
		//	default:
		//		break;   // a message that we don't process
		//	};
		}
		else
		{
			break;
		}
		iPrevMsgLen = m_iPacketLength;
		m_pLast = m_pBegin;
		m_pBegin += (m_iPacketLength + 2);

		if (!theApp.g_bReceiving)
			break;
	}; //  

	m_iLen = ulBytesLeft ;
	 // Adjust
	if (m_iLen)
	{
		memmove(&m_pszTVBuf[0], m_pBegin, m_iLen);			//Put the remaining "Partial message" in temp storage before clearing the buffer
 	}
	m_pBegin = NULL;

	return 1;   // for now until we document a return value
}
/////////////////////////////////////////////////////////////////////////////////////////////
int CReceiveITCH::ReadFromTestFile(char* strFileName)
{
	int	ihDatFile = -1;

	errno_t err = _sopen_s(&ihDatFile, strFileName, _O_RDWR | _O_BINARY |_O_SEQUENTIAL, _SH_DENYNO, 0 );  

	if	( err != 0 )
	{
		return TRUE; 
	}
// redundant...just for debug purposes
	switch (err)
	{
		case EACCES:
			return false;
		case EEXIST:
			return false;
		case EINVAL:
			return false;
		case EMFILE:
			return false;
		case ENOENT:
			return false;
	}
 	_int64	ulNumberOfBytes = 0;
	_int64	ulTotalBytes = 0; 

	m_iIteration = 0;
 
 	if (ihDatFile)
	{
		m_iLen = 0;
 		do
		{
			memset(m_pszRecvBuf, '\0', theApp.m_SOptions.dwBufferSize);

			ulNumberOfBytes = _read(ihDatFile, m_pszRecvBuf, theApp.m_SOptions.dwBufferSize);
			memmove(&(m_pszTVBuf[m_iLen]), m_pszRecvBuf, ulNumberOfBytes);		// Accumulate in m_szTVBuf

			m_iLen += ulNumberOfBytes;

			m_iIteration++;
			ulTotalBytes+= ulNumberOfBytes;

			ProcessFeed();

			if (!theApp.g_bReceiving)
				break;

		} while ((ulNumberOfBytes != -1) && (ulNumberOfBytes != 0) && (theApp.g_bReceiving));
	}
	_close(ihDatFile);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CReceiveITCH::~CReceiveITCH(void)
{
	//m_Socket = INVALID_SOCKET;
	//if (!bInflateEnd)
	//{
	//	(void)inflateEnd(&m_zstStream);
	//	bInflateEnd = true;
	//}

	if (m_pszTVBuf != NULL)
	{
		delete[] m_pszTVBuf;
 		m_pszTVBuf = NULL;
	}

	if (m_pszRecvBuf != NULL)
	{
		delete[] m_pszRecvBuf;
		m_pszRecvBuf = NULL;
	}

	if (m_pszOutputBuf != NULL)
	{
		delete[] m_pszOutputBuf;
		m_pszOutputBuf = NULL;
	}

	if (m_pFillStructs)
	{
		delete m_pFillStructs;
		m_pFillStructs = NULL;
	}
	if (m_pCUtil)
	{
		delete m_pCUtil;
		m_pCUtil = NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////