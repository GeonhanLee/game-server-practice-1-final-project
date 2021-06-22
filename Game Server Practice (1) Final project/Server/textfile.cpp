#include "textfile.h"

TextFile::TextFile(std::wstring fileName)
	: m_fileName(fileName)
{
	rw_mutex = CreateMutexW(NULL, FALSE, NULL);
	mutex = CreateMutexW(NULL, FALSE, NULL);

	readcount = 0;
}

TextFile::~TextFile()
{
	CloseHandle(rw_mutex);
	CloseHandle(mutex);
}

void TextFile::WaitMutex(HANDLE _mutex)
{
	WaitForSingleObject(_mutex, INFINITE);
}

void TextFile::SignalMutex(HANDLE _mutex)
{
	ReleaseMutex(_mutex);
}

void TextFile::OpenFin()
{
	fin.open(m_fileName);
}

void TextFile::CloseFin()
{
	fin.close();
}

void TextFile::OpenFout()
{

	fout.open(m_fileName, std::ios_base::app);
}

void TextFile::CloseFout()
{
	fout.close();
}

void TextFile::Read(std::wstring& out)
{
	WaitMutex(mutex);
	readcount++;
	if (readcount == 1)
		WaitMutex(rw_mutex);
	SignalMutex(mutex);

	OpenFin();
	std::wstringstream buffer;
	buffer << fin.rdbuf();
	out = buffer.str();
	CloseFin();

	WaitMutex(mutex);
	readcount--;
	if (readcount == 0)
		SignalMutex(rw_mutex);
	SignalMutex(mutex);
}

void TextFile::Write(const std::wstring& text)
{
	WaitMutex(rw_mutex);

	OpenFout();
	fout << text;
	CloseFout();

	SignalMutex(rw_mutex);
}
