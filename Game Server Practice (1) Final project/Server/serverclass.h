#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "textfile.h"
#define SERVERPORT 9000
#define BUFSIZE    512

class ServerClass
{
private:
	WSADATA wsa;
	SOCKET listen_sock;
	SOCKADDR_IN serveraddr;

	// ������ ��ſ� ����� ����
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

	typedef struct clientThreadParam {
		SOCKET clientSocket;
		TextFile* file;
	} ClientThreadParam;

	bool CreateProcessThread(_In_ LPTHREAD_START_ROUTINE lpStartAddress, ServerClass::ClientThreadParam* param);

	SOCKET GetClientSocket();
};

