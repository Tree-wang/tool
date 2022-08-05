#include "UDP_Class.h"
#include <iostream>

//创建新的套接字之前需要调用一个引入Ws2_32.dll库的函数,否则服务器和客户端连接不上
#pragma comment(lib,"ws2_32.lib")


UDP_Class::UDP_Class(int port)
{
	m_port = port;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa))
		throw std::exception("加载winsock动态链接库失败！");

	m_UDPSocket = socket(AF_INET,	//通信协议的协议族，对TCP/IP协议族，该参数设置为AF_INET
						SOCK_DGRAM,	//套接字类型为数据报套接字
						IPPROTO_UDP	//通信协议
						);
	if (m_UDPSocket == INVALID_SOCKET)
		throw std::exception("无效套接字！");

	//设置本地地址
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//监听端口号
	localAddr.sin_addr.s_addr = INADDR_ANY;	//本机

	if(0 != bind(m_UDPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//绑定地址
		throw std::exception("绑定地址失败！");
}


UDP_Class::~UDP_Class(void)
{
	closesocket(m_UDPSocket);
	WSACleanup();
}

bool UDP_Class::sendData(const char * ipDest, const char * buf, int len)
{
	SOCKADDR_IN destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(m_port);
	destAddr.sin_addr.s_addr = inet_addr(ipDest);

	if(SOCKET_ERROR  == sendto(m_UDPSocket,buf,len,0,(sockaddr*)&destAddr,sizeof(destAddr)))
		return false;
	else
		return true;
}

bool UDP_Class::receiveData(char * buf, int bufsize, int* len)
{
	SOCKADDR_IN fromAddr;
	*len = sizeof(fromAddr);
	if(SOCKET_ERROR  == recvfrom(m_UDPSocket,buf,bufsize,0,(sockaddr*)&fromAddr,len))
		return false;
	else
		return true;
}