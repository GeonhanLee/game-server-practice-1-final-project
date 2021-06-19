#include "textfile.h"
#include <codecvt>

TextFile::TextFile(std::wstring fileName)
	: m_fileName(fileName)
{
	m_mutex = CreateMutexW(NULL, FALSE, NULL);
}

TextFile::~TextFile()
{
	CloseHandle(m_mutex);
}

void TextFile::WaitMutex()
{
	WaitForSingleObject(m_mutex, INFINITE);
}

void TextFile::SignalMutex()
{
	ReleaseMutex(m_mutex);
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
	WaitMutex();
	OpenFin();

	std::wstringstream buffer;
	buffer << fin.rdbuf();
	out = buffer.str();

	CloseFin();
	SignalMutex();
}

void TextFile::Write(const std::wstring& text)
{
	WaitMutex();
	OpenFout();

	fout << text;

	CloseFout();
	SignalMutex();
}
