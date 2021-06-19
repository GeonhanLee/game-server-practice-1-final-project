#include <locale.h>
#include <iostream>
#include "textfile.h"

int wmain() {
	system("chcp 65001");
	system("cls");

	std::wstring fileName(L"file.txt"), fileContent;
	TextFile* file = new TextFile(fileName);

	file->Read(fileContent);

	std::wcout << L"File Content : " << fileContent << std::endl;

	delete file;
	return 0;
}