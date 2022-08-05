#include "TCP_class.h"
#include <iostream>
#include <Windows.h>
#include <process.h>

//�����µ��׽���֮ǰ��Ҫ����һ������Ws2_32.dll��ĺ���,����������Ϳͻ������Ӳ���
#pragma comment(lib,"ws2_32.lib")

TCPServer_class::TCPServer_class(int port)
{
	m_port = port;
	//m_connectCount = 0;

	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2,2),&wsa))
		throw std::exception("����winsock��̬���ӿ�ʧ�ܣ�");

	m_TCPSocket = socket(AF_INET,	//ͨ��Э���Э���壬��TCP/IPЭ���壬�ò�������ΪAF_INET
		SOCK_STREAM,	//�׽�������Ϊ������
		IPPROTO_TCP	//ͨ��Э��
		);
	if (m_TCPSocket == INVALID_SOCKET)
		throw std::exception("��Ч�׽��֣�");

	//���ñ��ص�ַ
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//�����˿ں�
	localAddr.sin_addr.s_addr = INADDR_ANY;	//����

	if(0 != bind(m_TCPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//�󶨵�ַ
		throw std::exception("�󶨵�ַʧ�ܣ�");

	if(SOCKET_ERROR == listen(m_TCPSocket,5))
		throw std::exception("����ʧ�ܣ�");
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
			//������WSAGetLastError��ȡ������󣬲�������Ӧ�Ĵ�����
			//�������Ϳɲο�MSDN

			closesocket(currentConnection);	//�Ͽ�����
		}
		else
		{
			//�Կͻ��˷�����������н���
			//////////////////////////
			if(SOCKET_ERROR == send(currentConnection,"feedback result",16,0))
				MessageBox(NULL,"������Ϣ����","����",MB_OKCANCEL);
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
		throw std::exception("����winsock��̬���ӿ�ʧ�ܣ�");

	m_TCPSocket = socket(AF_INET,	//ͨ��Э���Э���壬��TCP/IPЭ���壬�ò�������ΪAF_INET
		SOCK_STREAM,	//�׽�������Ϊ������
		IPPROTO_TCP	//ͨ��Э��
		);
	if (m_TCPSocket == INVALID_SOCKET)
		throw std::exception("��Ч�׽��֣�");

	//���ñ��ص�ַ
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//�����˿ں�
	localAddr.sin_addr.s_addr = INADDR_ANY;	//����

	if(0 != bind(m_TCPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//�󶨵�ַ
		throw std::exception("�󶨵�ַʧ�ܣ�");
}


TCPClient_class::~TCPClient_class(void)
{
	closesocket(m_TCPSocket);
	WSACleanup();
}

bool TCPClient_class::connect(const char * ipDest)
{
	//��дԶ�̵�ַ��Ϣ
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