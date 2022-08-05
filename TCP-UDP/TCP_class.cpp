#include "TCP_class.h"
#include <iostream>
#include <Windows.h>
#include <process.h>

//创建新的套接字之前需要调用一个引入Ws2_32.dll库的函数,否则服务器和客户端连接不上
#pragma comment(lib,"ws2_32.lib")

TCPServer_class::TCPServer_class(int port)
{
	m_port = port;
	//m_connectCount = 0;

	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2,2),&wsa))
		throw std::exception("加载winsock动态链接库失败！");

	m_TCPSocket = socket(AF_INET,	//通信协议的协议族，对TCP/IP协议族，该参数设置为AF_INET
		SOCK_STREAM,	//套接字类型为数据流
		IPPROTO_TCP	//通信协议
		);
	if (m_TCPSocket == INVALID_SOCKET)
		throw std::exception("无效套接字！");

	//设置本地地址
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//监听端口号
	localAddr.sin_addr.s_addr = INADDR_ANY;	//本机

	if(0 != bind(m_TCPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//绑定地址
		throw std::exception("绑定地址失败！");

	if(SOCKET_ERROR == listen(m_TCPSocket,5))
		throw std::exception("监听失败！");
}


TCPServer_class::~TCPServer_class(void)
{
	closesocket(m_TCPSocket);
	WSACleanup();
}

void TCPServer_class::handleConnections()
{
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);
	SOCKET currentConnection;
	while (1)
	{
		Sleep(500);
// 		if (m_connectCount > 5)
// 			continue;

		currentConnection = accept(m_TCPSocket,(SOCKADDR *)&remoteAddr,&nAddrLen);
		//m_connectCount++;
		_beginthread(handleRequests,0,(LPVOID)currentConnection);
	}
}

void TCPServer_class::handleRequests(LPVOID param)
{
	SOCKET currentConnection = (SOCKET)param;
	char buf[128];
	while (1)
	{
		if (SOCKET_ERROR == recv(currentConnection,buf,128,0))
		{
			//可利用WSAGetLastError获取具体错误，并进行相应的错误处理
			//错误类型可参考MSDN

			closesocket(currentConnection);	//断开连接
		}
		else
		{
			//对客户端发来的请求进行解析
			//////////////////////////
			if(SOCKET_ERROR == send(currentConnection,"feedback result",16,0))
				MessageBox(NULL,"接受消息错误！","警告",MB_OKCANCEL);
		}
	}
	//m_connectCount--;
}

//------------------------------------------------------------------------------------------------------------

TCPClient_class::TCPClient_class(int port)
{
	m_port = port;

	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2,2),&wsa))
		throw std::exception("加载winsock动态链接库失败！");

	m_TCPSocket = socket(AF_INET,	//通信协议的协议族，对TCP/IP协议族，该参数设置为AF_INET
		SOCK_STREAM,	//套接字类型为数据流
		IPPROTO_TCP	//通信协议
		);
	if (m_TCPSocket == INVALID_SOCKET)
		throw std::exception("无效套接字！");

	//设置本地地址
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//监听端口号
	localAddr.sin_addr.s_addr = INADDR_ANY;	//本机

	if(0 != bind(m_TCPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//绑定地址
		throw std::exception("绑定地址失败！");
}


TCPClient_class::~TCPClient_class(void)
{
	closesocket(m_TCPSocket);
	WSACleanup();
}

bool TCPClient_class::connect(const char * ipDest)
{
	//填写远程地址信息
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(m_port);
	servAddr.sin_addr.S_un.S_addr = inet_addr(ipDest);
	if (::connect(m_TCPSocket,(sockaddr *)&servAddr,sizeof(servAddr)) == -1)
		return false;
	else
		return true;
}

bool TCPClient_class::sendData(const char * buf, int len)
{
	if(SOCKET_ERROR == send(m_TCPSocket,buf,len,0))
		return false;
	else
		return true;
}

bool TCPClient_class::receiveData(char * buf,int len)
{
	if(SOCKET_ERROR == recv(m_TCPSocket,buf,len,0))
		return false;
	else
		return true;
}