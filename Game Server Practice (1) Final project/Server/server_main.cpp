#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")

#include <locale.h>
#include <iostream>
#include "packet.h"

#include "textfile.h"
#include "serverclass.h"
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	PacketSender* packetSender = new PacketSender(client_sock);
	PacketClass* packetClass = new PacketClass();

	TextFile* file = new TextFile(L"file.txt");

	while (1) {
		int retval = 0;
		retval = packetSender->RecievePacket(&(packetClass->packet));
		if (retval == SOCKET_ERROR) {
			// error
			break;
		}
		else if (retval == 0) {
			break;
		}

		std::wstring str;
		// ���� ����
		switch (packetClass->packet.header)
		{
		case PacketClass::Header::Req_Read:
			file->Read(str);
			std::wcout << L"READ : "<< str << std::endl;
			break;
		case PacketClass::Header::Req_Write:
			file->Write(packetClass->packet.data);
			file->Read(str);
			std::wcout << L"WRITE : " << str << std::endl;
			break;

		default:
			break;
		}


	}

	delete packetSender;
	delete packetClass;
	delete file;

	return 0;
}

int wmain() {

	system("chcp 65001");
	std::locale::global(std::locale(""));
	system("cls");


	ServerClass* server = new ServerClass();

	if (!server->Initialize()) {
		return 1;
	}

	while (1) {
		if (!server->AcceptClient()) {
			break;
		}

		//������ ����
		if (!server->CreateProcessThread(ProcessClient)) {
			server->CloseCurrentClientSocket();
		}
	}


	return 0;
}

void debug() {


	std::wstring fileName(L"file.txt"), fileContent;
	TextFile* file = new TextFile(fileName);

	//file->Write(L"����");
	file->Read(fileContent);

	std::wcout << L"File Content : " << fileContent << std::endl;

	delete file;

	auto packet = new PacketClass(PacketClass::Header::Null, L"");
	auto packetstruct = packet->packet;


}