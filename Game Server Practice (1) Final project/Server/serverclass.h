#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

class ServerClass
{
private:
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//char buf[BUFSIZE + 1];

public:
	ServerClass();
	~ServerClass();

	bool Initialize();

	bool AcceptClient();
	void CloseCurrentClientSocket();

	bool CreateProcessThread(_In_ LPTHREAD_START_ROUTINE lpStartAddress);

};

