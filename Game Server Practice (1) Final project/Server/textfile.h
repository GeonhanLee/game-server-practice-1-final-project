#pragma once

#include <windows.h>
#include <fstream>
#include <sstream>
#include <iostream>

class TextFile
{
public:
	TextFile(std::wstring fileName);
	~TextFile();
private:
	HANDLE m_mutex;
	void WaitMutex();
	void SignalMutex();

	std::wstring m_fileName;

	std::wifstream fin;
	std::wofstream fout;

	void OpenFin();
	void CloseFin();

	void OpenFout();
	void CloseFout();
public:
	void Read(std::wstring& out);
	void Write(const std::wstring& text);
};

