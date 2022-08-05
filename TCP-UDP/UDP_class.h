#pragma once
#include <winsock.h>
class UDP_Class
{
public:
	UDP_Class(int localPort);
	~UDP_Class(void);

	bool sendData(const char * ipDest, const char * buf, int len);
	bool receiveData(char * buf,int bufsize, int* len);

private:
	SOCKET m_UDPSocket;	//SOCKETÌ×½Ó×Ö
	int m_port;
};

