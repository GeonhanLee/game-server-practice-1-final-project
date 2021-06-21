#include "clientclass.h"

ClientClass::ClientClass()
{
}

ClientClass::~ClientClass()
{
}

bool ClientClass::Initialize()
{
    int retval;
    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;

    // socket()
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return false;

    // connect()
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) 
        return false;

    return true;
}

void ClientClass::CloseSocket()
{
    closesocket(sock);
}

SOCKET ClientClass::GetServerSocket()
{
    return sock;
}
