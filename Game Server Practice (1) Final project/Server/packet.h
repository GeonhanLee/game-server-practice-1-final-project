#pragma once
#include <string>

class PacketClass
{
public:
	enum class Header {
		Req_Con, Ack_Con, // ¿¬°á
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
			sizeof(PacketClass::Header) + 
			data.size() * sizeof(wchar_t) + 
			sizeof(unsigned char);
	}

};