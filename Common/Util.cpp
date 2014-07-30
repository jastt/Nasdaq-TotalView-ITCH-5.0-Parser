#include "stdafx.h"
#include "Util.h"
#include "Winsock2.h"

#define SIZEOFBUF 27

int iValue = 0;
unsigned long ulValue = 0;

unsigned _int64 i64Value = 0;
char strBuffer[SIZEOFBUF];
char strValue[256];  // sorry for the hard coded value
char cValue;

CUtil::CUtil(void)
{
	m_strValue.Empty();
}
//////////////////////////////////////////////////////////////////////////////
CUtil::~CUtil(void)
{
	int x = 0;
}
/////////////////////////////////////////////////////////////////////////////
char* CUtil::FloatToCharP(double fIn)
{
	memset(m_pChar, '\0', SIZE_OF_RET_BUFF);
	int	err = 0;

	err = _gcvt_s(m_pChar, SIZE_OF_RET_BUFF, fIn, 15);

	return m_pChar;
}
/////////////////////////////////////////////////////////////////////////////
char* CUtil::IntToCharP(int iIn)
{
	memset(m_pChar, '\0', SIZE_OF_RET_BUFF);
	int	err = 0;

	err = _itoa_s(iIn, m_pChar, 10);

	return m_pChar;
}
/////////////////////////////////////////////////////////////////////////////
CString CUtil::FloatToCString(double fIn)
{
	
	char szBuf[SIZEOFBUF];
	int	err = 0;

	m_strValue.Empty();

	memset(szBuf, '\0', SIZEOFBUF);

//	err = _gcvt_s(szBuf, SIZEOFBUF, fIn, 20);
//	err = _gcvt_s(szBuf, SIZEOFBUF, fIn, 3);

	sprintf_s(szBuf, SIZEOFBUF, "%.2f", fIn);

	if (err != 0)
	{
		return m_strValue;
	}
	//m_strValue.Format("%s", buf);
	
	m_strValue = szBuf;

	int istrLength = 0;
	CString strDec;

	if (m_strValue.Find(".") == -1)
		return m_strValue += ".00";

	int iRightToDec =  m_strValue.GetLength()-  m_strValue.Find(".") -1;
	strDec = m_strValue.Right(iRightToDec);
	istrLength = strDec.GetLength();
	switch (istrLength)
	{
	case 0:
		m_strValue += "00";
		break;
	case 1:
		m_strValue += "0";
		break;
	default:
		break;
	}
		
	return m_strValue;
}
/////////////////////////////////////////////////////////////////////////////
CString CUtil::IntToCString(int iIn)
{
	char buf[SIZEOFBUF];
	int	err = 0;

	m_strValue.Empty();
	memset(buf, '\0', SIZEOFBUF);

	err = _itoa_s(iIn, buf, 10);

	if (err != 0)
	{
		return m_strValue;
	}
	m_strValue.Format("%s", buf);

	return m_strValue;
}
/////////////////////////////////////////////////////////////////////////////
CString CUtil::UnsingedToCString(unsigned uIn)
{
	char buf[SIZEOFBUF];
	int	err = 0;

	m_strValue.Empty();
	memset(buf, '\0', SIZEOFBUF);

	err = _ultoa_s(uIn, buf, 10);

	if (err != 0)
	{
		return m_strValue;
	}
	m_strValue.Format("%s", buf);

	return m_strValue;
}
/////////////////////////////////////////////////////////////////////////////
double  CUtil::CStringToFloat(CString strIn)
{
	double fReturn = 0;
	char buf[SIZEOFBUF];

	if (strIn.GetLength() > (SIZEOFBUF -2))
		return 0;

	memset(buf, '\0', SIZEOFBUF);
	strcpy_s(buf, strIn.GetString());
	fReturn = atof(buf);

	return fReturn;
}
/////////////////////////////////////////////////////////////////////////////
unsigned int CUtil::CStringToUI(CString strIn)
{
	unsigned int uiReturn = 0;
	char buf[SIZEOFBUF];

	if (strIn.GetLength() > (SIZEOFBUF -2))
		return 0;
	memset(buf, '\0', SIZEOFBUF);
	strcpy_s(buf, strIn.GetString());
	uiReturn = atoi(buf);

	return uiReturn;
}
/////////////////////////////////////////////////////////////////////////////
CString CUtil::PadCString(CString strIn)
{
	int istrLength = 0;
	CString strDec;

	if (strIn.Find(".") == -1)
		return strIn += ".00";

	int iRightToDec =  strIn.GetLength()-  strIn.Find(".") -1;
	strDec = strIn.Right(iRightToDec);
	istrLength = strDec.GetLength();
	switch (istrLength)
	{
	case 0:
		strIn += "00";
		break;
	case 1:
		strIn += "0";
		break;
	default:
		break;
	}
	return strIn;
}
/////////////////////////////////////////////////////////////////////////////
double  CUtil::GetValueDouble(UINT8 *uiMsg, int iOffset, int iLength)
{
	double dValue = 0;
	/*
	memset(strBuffer, '\0', SIZEOFBUF);
	if (iLength >= SIZEOFBUF)
		iLength = SIZEOFBUF -1;
	memmove(strBuffer,  uiMsg +iOffset,  iLength);  
	dValue = atof(strBuffer);
	*/
	return dValue;
} 
////////////////////////////////////////////////////////////////////////////
short	CUtil::GetValueShort(UINT8 *uiMsg, int iOffset, int iLength)
{
	unsigned short uiBuff = 0;

 	memmove(&uiBuff,  uiMsg +iOffset,  iLength);   
	iValue = ntohs(uiBuff);

	return iValue;
}
////////////////////////////////////////////////////////////////////////////
int	CUtil::GetValueInt(UINT8 *uiMsg, int iOffset, int iLength)
{
	int uiBuff = 0;

	memmove(&uiBuff,  uiMsg +iOffset,  iLength);   
	iValue = ntohl(uiBuff);

	return iValue;
}
////////////////////////////////////////////////////////////////////////////
unsigned long	CUtil::GetValueUnsignedLong(UINT8 *uiMsg, int iOffset, int iLength)
{
	unsigned long ulBuff = 0;

 	memmove(&ulBuff,  uiMsg +iOffset,  iLength);   
	ulValue = ntohl(ulBuff);

	return ulValue;
}
////////////////////////////////////////////////////////////////////////////
#define _WS2_32_WINSOCK_SWAP_LONGLONG(l)            \
            ( ( ((l) >> 56) & 0x00000000000000FFLL ) |       \
              ( ((l) >> 40) & 0x000000000000FF00LL ) |       \
              ( ((l) >> 24) & 0x0000000000FF0000LL ) |       \
              ( ((l) >>  8) & 0x00000000FF000000LL ) |       \
              ( ((l) <<  8) & 0x000000FF00000000LL ) |       \
              ( ((l) << 24) & 0x0000FF0000000000LL ) |       \
              ( ((l) << 40) & 0x00FF000000000000LL ) |       \
              ( ((l) << 56) & 0xFF00000000000000LL ) )

///////////////////////////////////////////////////////////////////////////
unsigned _int64	CUtil::GetValueUnsignedInt64(UINT8 *uiMsg, int iOffset, int iLength)
{
	unsigned _int64 i64Buff = 0;

	memset(&i64Buff, '\0', 8);
	memmove(&i64Buff,  uiMsg +iOffset,  iLength);   

 	i64Value = _WS2_32_WINSOCK_SWAP_LONGLONG(i64Buff);

//	GetTimeFromNano(i64Value);

	return i64Value;
}
////////////////////////////////////////////////////////////////////////////
char*   CUtil::GetValueAlpha(UINT8 *uiMsg, int iOffset, int iLength)
{
//	char strValue[256];  // sorry for the hard coded value
 	memset(strValue, '\0', 256);
	if (iLength > 255)
		iLength = 255;

	memmove(strValue,  uiMsg +iOffset,  iLength);  

	return strValue;
}
////////////////////////////////////////////////////////////////////////////
char    CUtil::GetValueChar(UINT8 *uiMsg, int iOffset, int iLength)
{
	cValue = '\0';
	cValue = uiMsg[iOffset];

	return cValue;
}
////////////////////////////////////////////////////////////////////////////

 