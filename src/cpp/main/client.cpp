#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>

#include "net_setting.h"

#include <iostream>
#include <string>

void get_message(char* out) {
	std::string in;
	while (in.size() == 0) {
		printf("client:");
		std::getline(std::cin, in);
	}
	strcpy(out, in.c_str());
}

int main(int argc, char* argv[])
{
 	SetConsoleOutputCP(CP_UTF8);
	//初始化网络环境
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
	// 初始化完成，创建一个TCP的socket
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		return -1;
	}
	//指定连接的服务端信息
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(SERVER_NET_PORT);
	//客户端只需要连接指定的服务器地址，127.0.0.1是本机的回环地址
	addrServ.sin_addr.S_un.S_addr = inet_addr(SERVER_IPV4);

	// 服务器Bind 客户端是进行连接
	int ret = connect(sServer, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));//开始连接
	if (SOCKET_ERROR == ret)
	{
		printf("socket connect failed\n");
		WSACleanup();
		closesocket(sServer);
		return -1;
	}
	//连接成功后，就可以进行通信了
	printf("connect to server\n");
	printf("一次只能说一句话嗷，输入回车发送~\n");
	printf("输入\"#bye~\"结束会话\n");
	printf("server:猜猜谁在和你说话\n");
	char szBuf[1024];
	while (1) {
		memset(szBuf, 0, sizeof(szBuf));
		get_message(szBuf);
		int rc = send(sServer, szBuf, strlen(szBuf), 0);

		if (rc == SOCKET_ERROR or rc == 0 or strcmp(szBuf, "#bye~\0") == 0) {
			break;
		}

		memset(szBuf, 0, sizeof(szBuf));
		rc = recv(sServer, szBuf, sizeof(szBuf), 0);
		printf("server:%s\n", szBuf);
		if (rc == SOCKET_ERROR or strcmp(szBuf, "#bye~\0") == 0) {
			break;
		}
	}
	//关闭连接
	printf("close connect\n");
	closesocket(sServer);
	WSACleanup();
	system("pause");
	return 0;
}