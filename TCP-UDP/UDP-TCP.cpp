// UDP.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UDP_Class.h"
#include "TCP_class.h"

#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
// 	UDP_Class udp(777);
// 	char tmp[128], buf1[128];
// 	while(1)
// 		udp.sendData("172.21.255.255","hello",6);
// 
// 	char buf2[128]="";
// 	int len;
// 	while(1)
// 	{
// 		udp.receiveData(buf2,128,&len);
// 		cout<<buf2<<endl;
// 	}
// 	system("pause");

// 	TCPServer_class tcpServer(565);
// 	tcpServer.handleConnections();

	TCPClient_class tcpClient(565);
	tcpClient.connect("172.21.128.4");
	while(1)
	{
		Sleep(1000);
		tcpClient.sendData("hello server",13);
		char buf[128];
		tcpClient.receiveData(buf,128);
		cout<<buf<<endl;
		//break;
	}

	return 0;
}

