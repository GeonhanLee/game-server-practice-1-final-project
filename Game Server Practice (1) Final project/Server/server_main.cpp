#define WIN32_LEAN_AND_MEAN
#include <locale.h>
#include <iostream>
#include "packet.h"

#include "textfile.h"
#include "serverclass.h"
DWORD WINAPI ProcessClient(LPVOID arg)
{
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

		//스레드 생성
		if (!server->CreateProcessThread(ProcessClient)) {
			server->CloseCurrentClientSocket();
		}
	}


	return 0;
}

void debug() {


	std::wstring fileName(L"file.txt"), fileContent;
	TextFile* file = new TextFile(fileName);

	//file->Write(L"ㅎㅎ");
	file->Read(fileContent);

	std::wcout << L"File Content : " << fileContent << std::endl;

	delete file;

	auto packet = new PacketClass(PacketClass::Header::Null, L"");
	auto packetstruct = packet->packet;


}