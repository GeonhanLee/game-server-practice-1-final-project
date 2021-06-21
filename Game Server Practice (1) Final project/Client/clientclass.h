#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512
class ClientClass
{
private:
	WSADATA wsa;
	SOCKET sock;
	SOCKADDR_IN serveraddr;

public:
	ClientClass();
	~ClientClass();

	bool Initialize();

	void CloseSocket();

	SOCKET GetServerSocket();
};

