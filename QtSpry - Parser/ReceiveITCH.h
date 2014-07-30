#pragma once


#include  "Util.h"

#include  "Includes.h"
#include  "FillMsgStructs.h"

#define	MSG_LENGTH	255

#define	COMP_FACTOR			10		// compression factor

class CReceiveITCH
{
	SOCKET m_Socket;
public:
	CReceiveITCH(SOCKET* );
	virtual ~CReceiveITCH(void);

	int ReadFromTestFile(char* strFileName);

	int ProcessFeed();


	int			m_iTVBufLen;
	int			m_iOutputBuffLen;
	int			m_pszRecvBufLen;

	
  char		szMsg[MSG_LENGTH];


	  UINT8		*m_pLast;
	  UINT8		*m_pBegin;
	_int64			m_iLen ;

	UINT8 		*m_pszTVBuf;
	  UINT8		*m_pszRecvBuf;
	  UINT8		*m_pszOutputBuf;

	UINT8		m_szMessage[SOUP_MESSAGE_SIZE];   // each individual message


private:
		int m_iPacketLength;
		int m_iPacketType;

		UINT8 m_strPacketLength[3];
		UINT8 m_strPacketType[2];

		CFillMsgStructs *m_pFillStructs;
		CUtil	*m_pCUtil;

		int m_iIteration;


};

