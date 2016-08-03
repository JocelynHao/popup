#pragma once

class CBONCSocket
{
public:
	CBONCSocket(void);
	virtual ~CBONCSocket(void);

public:
	BOOL Create(const int nSocketType = SOCK_STREAM);

	BOOL Connect(LPCTSTR lpszHost, const WORD wPort);
	void Close();

	int  Write(const char* pchBuf, const int nSize, const int nTimeout = 10/*in seconds*/);
	int  Receive(char* pchBuf, const int nSize, const int nTimeout = 10/*in seconds*/);

	int  GetLocalIPList(CStringArray &saIP);

protected:
	BOOL Initialize(void);
	void Uninitialize(void);
	int  Send(const char* pchBuf, const int nSize, const int nTimeout);

protected:
	DWORD	m_dwErrorCode;	// Í¨Ñ¶´íÎóÂë
	SOCKET	m_hSocket;
	BOOL	m_bInited;
	BOOL	m_bConnected;
};

