#pragma once

#include <WinSock.h>

class TCPServer_class
{
public:
	TCPServer_class(int port);
	~TCPServer_class(void);

	void handleConnections();
	static void handleRequests(LPVOID);

private:
	SOCKET m_TCPSocket;
	int m_port;

	static int m_connectCount;
};

class TCPClient_class
{
public:
	TCPClient_class(int port);
	~TCPClient_class(void);

	bool connect(const char * ipDest);
	bool sendData(const char * buf, int len);
	bool receiveData(char * buf, int len);
private:
	SOCKET m_TCPSocket;
	int m_port;
};
