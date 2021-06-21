#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")

#include <locale.h>
#include <iostream>
#include <conio.h>
#include "packet.h"
#include "clientclass.h"
int wmain() {
	std::locale::global(std::locale(""));

	ClientClass* client = new ClientClass();

	if (!client->Initialize()) {
		return 1;
	}

	PacketSender* packetSender = new PacketSender(client->GetServerSocket());
	PacketClass* packetClass = nullptr;// = new PacketClass(PacketClass::Header::Req_Write, L"��");

	while (1) {

		std::wcout << L"1. �б�  2: ����  3: ����" << std::endl;
		int inputNum = 0;
		do {
			inputNum = _getch();
		} while ( !(inputNum == (int)'1' || inputNum == (int)'2' || inputNum == (int)'3') );

		if (inputNum == (int)'1') { // �б�
			packetClass = new PacketClass(PacketClass::Header::Req_Read, L"");
		}
		else if (inputNum == (int)'2') { // ����
			std::wstring inputStr;
			std::wcout << L"������ �Է��ϼ��� : ";
			std::wcin >> inputStr;
			packetClass = new PacketClass(PacketClass::Header::Req_Write, inputStr);
		}
		else if (inputNum == (int)'3') { // ����
			break;
		}
		else {
			printf("err");
			break;
		}

		// ��Ŷ �߽�
		int retval = 0;
		retval = packetSender->SendPacket(&(packetClass->packet));
		if (retval == SOCKET_ERROR) {
			// error
			break;
		}
		else if (retval == 0) {
			break;
		}

		// ��� ��Ŷ ����
		retval = packetSender->RecievePacket(&(packetClass->packet));
		if (retval == SOCKET_ERROR) {
			// error
			break;
		}
		else if (retval == 0) {
			break;
		}

		switch (packetClass->packet.header)
		{
		case PacketClass::Header::Ack_Read:
			std::wcout << L"�б� ����" << std::endl;
			break;
		case PacketClass::Header::Ack_Write:
			std::wcout << L"���� ����" << std::endl;
			break;
		default:
			break;
		}
		std::wcout << L"��� : " << packetClass->packet.data << std::endl << std::endl;

		if (!packetClass)
			delete packetClass;
	}
	client->CloseSocket();
	if (!packetClass)
		delete packetClass;
	delete client;

	return 0;
}