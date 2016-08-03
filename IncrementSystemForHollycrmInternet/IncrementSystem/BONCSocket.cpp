#include "StdAfx.h"
#include "BONCSocket.h"


CBONCSocket::CBONCSocket(void)
{
	m_dwErrorCode = 0;
	m_hSocket = INVALID_SOCKET;
	m_bInited = FALSE;
	m_bConnected = FALSE;

	m_bInited = Initialize();
}

CBONCSocket::~CBONCSocket(void)
{
	Close();
	Uninitialize();
}

BOOL CBONCSocket::Create(const int nSocketType)
{
	if (!m_bInited) return FALSE;

	m_hSocket = socket(AF_INET, nSocketType, IPPROTO_TCP);
	if (m_hSocket == INVALID_SOCKET) 
	{
		m_dwErrorCode = WSAGetLastError(); 
		TRACE(_T("Filed to call socket()!The error code is %d.\n"),m_dwErrorCode);
		return FALSE;
	}

	return TRUE;
}

BOOL CBONCSocket::Connect(LPCTSTR lpszHost, const WORD wPort)
{
	USES_CONVERSION_EX;

	sockaddr_in sockAddr = {0};
	LPSTR		lpszAscii = NULL;
	LPHOSTENT	lphostHost = NULL;

	if (m_hSocket == INVALID_SOCKET) return FALSE;

	if (lpszHost == NULL)
	{
		WSASetLastError(WSAEINVAL);
		TRACE(_T("Invalid argument.\n"));
		return FALSE;
	}

	lpszAscii = T2A_EX((LPTSTR)lpszHost, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
	if (lpszAscii == NULL)
	{
		WSASetLastError(ERROR_NOT_ENOUGH_MEMORY);
		TRACE(_T("Not enough storage is available.\n"));
		return FALSE;
	}

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
	sockAddr.sin_port = htons(wPort);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		lphostHost = gethostbyname(lpszAscii);
		if (lphostHost != NULL)
		{
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphostHost->h_addr)->s_addr;
		}
		else
		{
			TRACE(_T("Invalid argument.\n"));
			return FALSE;
		}
	}

	// should timeout by itself
	if (connect(m_hSocket, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		m_dwErrorCode = WSAGetLastError(); 
		closesocket(m_hSocket);
		TRACE(_T("Filed to call connect()!The error code is %d.\n"),m_dwErrorCode);
		return FALSE;
	}

	m_bConnected = TRUE;

	return TRUE;
}

void CBONCSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		if (closesocket(m_hSocket) == SOCKET_ERROR)
		{
			m_dwErrorCode = WSAGetLastError();
			TRACE(_T("Filed to call closesocket()!The error code is %d.\n"),m_dwErrorCode);
		}
	}

	m_bConnected = FALSE;
	m_hSocket = INVALID_SOCKET;
}

int CBONCSocket::Write(const char* pchBuf, const int nSize, const int nTimeout)
{
	int	nBytesSent = 0;
	int	nBytesThisTime = 0;

	if (!m_bConnected) return 0;

	do
	{
		nBytesThisTime = Send(pchBuf + nBytesSent, nSize - nBytesSent, nTimeout);
		if (nBytesThisTime < 1) return nBytesSent;

		nBytesSent += nBytesThisTime;
	} while(nBytesSent < nSize);

	return nBytesSent;
}

int CBONCSocket::Receive(char* pchBuf, const int nSize, const int nTimeout)
{
	FD_SET	fdsRead = {1, m_hSocket};
	TIMEVAL	tvTimeout = {nTimeout, 0};
	int		nBytesReceived = 0;
	int		nResult = 0;

	if (!m_bConnected) return FALSE;

	nResult = select(0, &fdsRead, NULL, NULL, &tvTimeout);
	if (nResult == 0)
	{
		TRACE(_T("The time limit expired.\n"));
		return 0;
	}
	else if (nResult == SOCKET_ERROR)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call select()!The error code is %d.\n"),m_dwErrorCode);
		return SOCKET_ERROR;
	}

	if ((nBytesReceived = recv(m_hSocket, pchBuf, nSize, 0)) == SOCKET_ERROR )
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call recv()!The error code is %d.\n"),m_dwErrorCode);
		return SOCKET_ERROR;
	}

	return nBytesReceived;
}

int CBONCSocket::GetLocalIPList(CStringArray &saIP)
{
	CString strIP = _T("");
	HOSTENT *host = NULL;
	char	szHostname[128] = {0}; 
	int		nStatus = 0; 

	nStatus = gethostname(szHostname, sizeof(szHostname)); 
	if (nStatus == SOCKET_ERROR)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call gethostname()!The error code is %d.\n"),m_dwErrorCode);
		return 0;
	}

	host = gethostbyname(szHostname); 
	if (host == NULL) 
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call gethostbyname()!The error code is %d.\n"),m_dwErrorCode);
		return 0;
	}

	for (int i = 0; ;i++) 
	{ 
		strIP = inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]); 

		TRACE(_T("IP%d = %s\n"),i,strIP);
		saIP.Add(strIP);

		if (host->h_addr_list[i] + host->h_length >= host->h_name) 
		{	
			TRACE(_T("The number of local IP is %d.\n"),i + 1);
			break; 
		}
	}

	return saIP.GetSize();
}

BOOL CBONCSocket::Initialize(void)
{
	if (m_bInited) return TRUE;

	WSADATA wsaData = {0};
	WORD	wdVersionRequested = MAKEWORD(2, 2);

	TRACE(_T("Initiating use of the Winsock 2 DLL ...\n"));

	if (WSAStartup(wdVersionRequested, &wsaData) != 0)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Could not find a usable WinSock DLL!The error code is %d.\n"),m_dwErrorCode);
		return FALSE;
	}

	// Confirm that the WinSock DLL supports 2.2.
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		TRACE(_T("Incorrect version of WinSock DLL found!\n"));
		WSACleanup();
		return FALSE;
	}

	m_bInited = TRUE;
	TRACE(_T("The WinSock DLL is acceptable.\n"));

	return TRUE;
}

void CBONCSocket::Uninitialize(void)
{
	TRACE(_T("Terminating use of the Winsock 2 DLL ...\n"));

	if (WSACleanup() != 0)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Could not find a usable WinSock DLL!The error code is %d.\n"),m_dwErrorCode);
	}

	m_bInited = FALSE;
}

int  CBONCSocket::Send(const char* pchBuf, const int nSize, const int nTimeout)
{
	if (m_hSocket == INVALID_SOCKET) return FALSE;

	// returned value will be less than nSize if client cancels the reading
	FD_SET	fdsWrite = {1, m_hSocket};
	TIMEVAL	tvTimeout = {nTimeout, 0};
	int		nBytesSent = 0;
	int		nResult = 0;

	nResult = select(0, NULL, &fdsWrite, NULL, &tvTimeout);

	if (nResult == 0)
	{
		TRACE(_T("The time limit expired.\n"));
		return 0;
	}
	else if (nResult == SOCKET_ERROR)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call select()!The error code is %d.\n"),m_dwErrorCode);
		return SOCKET_ERROR;
	}

	if ((nBytesSent = send(m_hSocket, pchBuf, nSize, 0)) == SOCKET_ERROR)
	{
		m_dwErrorCode = WSAGetLastError();
		TRACE(_T("Filed to call send()!The error code is %d.\n"),m_dwErrorCode);
		return SOCKET_ERROR;
	}

	return nBytesSent;
}