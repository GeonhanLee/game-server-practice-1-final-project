#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
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
	PacketClass* packetClass = nullptr;// = new PacketClass(PacketClass::Header::Req_Write, L"ㅋ");

	while (1) {

		std::wcout << L"1. 읽기  2: 쓰기  3: 종료" << std::endl;
		int inputNum = 0;
		do {
			inputNum = _getch();
		} while ( !(inputNum == (int)'1' || inputNum == (int)'2' || inputNum == (int)'3') );

		if (inputNum == (int)'1') { // 읽기
			packetClass = new PacketClass(PacketClass::Header::Req_Read, L"");
		}
		else if (inputNum == (int)'2') { // 쓰기
			std::wstring inputStr;
			std::wcout << L"내용을 입력하세요 : ";
			std::wcin >> inputStr;
			packetClass = new PacketClass(PacketClass::Header::Req_Write, inputStr);
		}
		else if (inputNum == (int)'3') { // 종료
			break;
		}
		else {
			printf("err");
			break;
		}

		// 패킷 발신
		int retval = 0;
		retval = packetSender->SendPacket(&(packetClass->packet));
		if (retval == SOCKET_ERROR) {
			// error
			break;
		}
		else if (retval == 0) {
			break;
		}

		// 결과 패킷 수신
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
			std::wcout << L"읽기 성공" << std::endl;
			break;
		case PacketClass::Header::Ack_Write:
			std::wcout << L"쓰기 성공" << std::endl;
			break;
		default:
			break;
		}
		std::wcout << L"결과 : " << packetClass->packet.data << std::endl << std::endl;

		if (!packetClass)
			delete packetClass;
	}
	client->CloseSocket();
	if (!packetClass)
		delete packetClass;
	delete client;

	return 0;
}