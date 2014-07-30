#pragma once

#define		INVALID_INPUT	-1
#define		SIZE_OF_TEMP_BUFF	128
#define		SIZE_OF_RET_BUFF	25

class CUtil
{
private:
	char	m_pChar[SIZE_OF_RET_BUFF];

public:
	CUtil(void);
	~CUtil(void);

	CString		FloatToCString(double fIn);
	CString		IntToCString(int iIn);
	CString		UnsingedToCString(unsigned uIn);

	char*		FloatToCharP(double fIn);
	char*		IntToCharP(int iIn);

	CString		PadCString(CString srtIn);

	double		CStringToFloat(CString strIn);
	unsigned	int CStringToUI(CString strIn);

	CString		m_strValue;

// Helper function for parsing the ITCH Messages
	double			GetValueDouble(UINT8 *uiMsg, int iOffset, int iLength);
	
	short 			GetValueShort(UINT8 *uiMsg, int iOffset, int iLength);
	int				GetValueInt(UINT8 *uiMsg, int iOffset, int iLength);
	unsigned _int64	GetValueUnsignedInt64(UINT8 *uiMsg, int iOffset, int iLength);
	unsigned long	GetValueUnsignedLong(UINT8 *uiMsg, int iOffset, int iLength);
	char*			GetValueAlpha(UINT8 *uiMsg, int iOffset, int iLength);
	char			GetValueChar(UINT8 *uiMsg, int iOffset, int iLength);

};
