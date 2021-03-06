#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")

#include <locale.h>
#include <iostream>
#include "packet.h"

#include "textfile.h"
#include "serverclass.h"
DWORD WINAPI ProcessClient(LPVOID arg)
{
	auto param = (ServerClass::ClientThreadParam*)arg;

	SOCKET client_sock = param->clientSocket;
	TextFile* file = param->file;

	PacketSender* packetSender = new PacketSender(client_sock);
	PacketClass* packetClass = new PacketClass();

	while (1) {
		// 패킷 수신
		int retval = 0;
		retval = packetSender->RecievePacket(&(packetClass->packet));
		if (retval == SOCKET_ERROR) {
			// error
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 파일 수정
		std::wstring str;
		switch (packetClass->packet.header)
		{
		case PacketClass::Header::Req_Read:
			file->Read(str);
			std::wcout << L"READ : " << str << std::endl;
			break;
		case PacketClass::Header::Req_Write:
			file->Write(packetClass->packet.data);
			file->Read(str);
			std::wcout << L"WRITE : " << str << std::endl;
			break;
		default:
			break;
		}

		// 결과 패킷 발신
		PacketClass* returnPacketClass = nullptr;
		file->Read(str);
		switch (packetClass->packet.header)
		{
		case PacketClass::Header::Req_Read:
			returnPacketClass = new PacketClass(PacketClass::Header::Ack_Read, str);
			break;
		case PacketClass::Header::Req_Write:
			returnPacketClass = new PacketClass(PacketClass::Header::Ack_Write, str);
			break;
		default:
			break;
		}

		retval = packetSender->SendPacket(&(returnPacketClass->packet));
		if (retval == SOCKET_ERROR) {
			// error

			if (returnPacketClass) delete returnPacketClass;
			break;
		}
		else if (retval == 0) {
			if (returnPacketClass) delete returnPacketClass;
			break;
		}
		if (returnPacketClass) delete returnPacketClass;
	}
	delete packetSender;
	delete packetClass;

	return 0;
}

int wmain() {
	std::locale::global(std::locale(""));

	ServerClass* server = new ServerClass();
	TextFile* file = new TextFile(L"file.txt");

	if (!server->Initialize()) {
		return 1;
	}

	while (1) {
		if (!server->AcceptClient()) {
			break;
		}

		auto clientThreadParam = ServerClass::ClientThreadParam{ server->GetClientSocket(), file};

		//스레드 생성
		if (!server->CreateProcessThread(ProcessClient, &clientThreadParam)) {
			server->CloseCurrentClientSocket();
		}
	}

	delete server;
	delete file;
	return 0;
}