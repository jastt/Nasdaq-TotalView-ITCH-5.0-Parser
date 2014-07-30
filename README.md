Nasdaq-TotalView-ITCH-5.0-Parser
================================


This project is meant for market data professionals and enthusiasts implementing NASDAQ TotalView ITCH 5.0 specification (http://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf)

With a large buffer size, the parser is capable of handling over 10 million msgs/sec on commodity hardware (8GB RAM/Win7 64)


Please build the files according to the following:


-Visual Studio 11 (2012)

-Get ITCH 5.0 file from: ftp://emi.nasdaq.com/ITCH/06022014.NASDAQ_ITCH50.gz

-Place file in "C:\ITCH5.0TestFeed\" or put in directory of your choice and change line 402 in NQTVDlg.cpp (“char strFileName[MAX_PATH] = {"C:\\ITCH5.0TestFeed\\06022014.NASDAQ_ITCH50"}”)

-To change the buffer size: NQTV.cpp Line 26 ("m_SOptions.dwBufferSize = 1000000")


Running the application:


-Click the \x64 executable 

-When the dialog opens, click "Replay"

-Click "Start" Feed


Note: The other buttons on the main dialog are place holders. 
