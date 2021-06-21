#pragma once
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#include <winsock2.h>

#define BUFSIZE    512

class PacketClass
{
public:
	enum class Header {
		Req_Con, Ack_Con, // 연결
		Req_Read, Ack_Read,
		Req_Write, Ack_Write,
		Null
	};

	typedef struct packet {
		int length;
		PacketClass::Header header;
		std::wstring data;
		unsigned char end;
	} Packet;

	Packet packet;
public:
	PacketClass(PacketClass::Header header, std::wstring data) 
		: packet{ 0, header, data, 0xff }
	{
		SetLength(data);
	}
	~PacketClass() {

	}

private:
	void SetLength(const std::wstring& data) {
		packet.length = 
			sizeof(int)+
			sizeof(PacketClass::Header) + 
			data.size() * sizeof(wchar_t) + 
			sizeof(unsigned char);
	}

};

class PacketSender
{
private:
	SOCKET targetSocket;
	SOCKADDR_IN targetAddr;
	int addrlen;

	//char buf[BUFSIZE + 1];
public:
	PacketSender(SOCKET target) {
		targetSocket = target;
		addrlen = sizeof(targetAddr);
		getpeername(targetSocket, (SOCKADDR*)&targetAddr, &addrlen);
	}
	~PacketSender() {
		closesocket(targetSocket);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(targetAddr.sin_addr), ntohs(targetAddr.sin_port));
	}
private:
	// 사용자 정의 데이터 수신 함수
	int recvn(SOCKET s, char* buf, int len, int flags)
	{
		int received;
		char* ptr = buf;
		int left = len;

		while (left > 0) {
			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;
			left -= received;
			ptr += received;
		}

		return (len - left);
	}
public:
	int SendPacket(PacketClass::Packet* packet) {
		int retval = 0;
		retval = send(targetSocket, (char*)packet->length, sizeof(packet->length), 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			return retval;
		}

		retval = send(targetSocket, (char*)packet, packet->length, 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			return retval;
		}

		return retval;
	}

	int RecievePacket(PacketClass::Packet* outPacket) {
		int retval = 0;
		retval = recvn(targetSocket, (char*)outPacket->length, sizeof(outPacket->length), 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			return retval;
		}

		retval = recvn(targetSocket, (char*)outPacket, outPacket->length, 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			return retval;
		}

		return retval;
	}
}; 