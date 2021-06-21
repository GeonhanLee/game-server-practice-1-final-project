#include "serverclass.h"
//#include <windows.h>
ServerClass::ServerClass()
{
}

ServerClass::~ServerClass()
{
}

bool ServerClass::Initialize()
{
    int retval;
    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;

    // socket()
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
        return false;

    // bind()
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) 
        return false;

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) 
        return false;

    return true;
}

bool ServerClass::AcceptClient()
{

    // accept()
    addrlen = sizeof(clientaddr);
    client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

    if (client_sock == INVALID_SOCKET) {
        return false;
    }

    // 접속한 클라이언트 정보 출력
    printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return true;
}

void ServerClass::CloseCurrentClientSocket()
{
    closesocket(client_sock);
}

bool ServerClass::CreateProcessThread(_In_ LPTHREAD_START_ROUTINE lpStartAddress)
{
    HANDLE hThread;
    hThread = CreateThread(NULL, 0, lpStartAddress,
        (LPVOID)client_sock, 0, NULL);
    if (hThread == NULL) {
        return false;
    }
    CloseHandle(hThread);
    return true;
}
