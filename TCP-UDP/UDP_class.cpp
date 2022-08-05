#include "UDP_Class.h"
#include <iostream>

//�����µ��׽���֮ǰ��Ҫ����һ������Ws2_32.dll��ĺ���,����������Ϳͻ������Ӳ���
#pragma comment(lib,"ws2_32.lib")


UDP_Class::UDP_Class(int port)
{
	m_port = port;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa))
		throw std::exception("����winsock��̬���ӿ�ʧ�ܣ�");

	m_UDPSocket = socket(AF_INET,	//ͨ��Э���Э���壬��TCP/IPЭ���壬�ò�������ΪAF_INET
						SOCK_DGRAM,	//�׽�������Ϊ���ݱ��׽���
						IPPROTO_UDP	//ͨ��Э��
						);
	if (m_UDPSocket == INVALID_SOCKET)
		throw std::exception("��Ч�׽��֣�");

	//���ñ��ص�ַ
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(m_port);	//�����˿ں�
	localAddr.sin_addr.s_addr = INADDR_ANY;	//����

	if(0 != bind(m_UDPSocket,(sockaddr*)&localAddr,sizeof(localAddr)))	//�󶨵�ַ
		throw std::exception("�󶨵�ַʧ�ܣ�");
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