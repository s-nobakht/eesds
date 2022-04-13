#include "include.h"

InputStream::InputStream()
{
	cBuffer = NULL;
	closed = 1;
}

InputStream::InputStream(TCHAR *tPath, int nSize)
{
	menosuno = 0;
	closed = 0;
	bytesRead = 0;
	this->nSize = nSize;
	cBuffer = new char[nSize];
	cCrtBuffer = cBuffer + nSize;
	hFile = CreateFile(tPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		throw FILEOPENPROBLEM;
}

InputStream::~InputStream()
{
	if (cBuffer)
		delete[] cBuffer;
	if (!closed)
		close();
}

int InputStream::read()
{
	if (menosuno)
		return -1;
	if ((DWORD)(cCrtBuffer - cBuffer) >= bytesRead)
	{
		if (ReadFile(hFile, cBuffer, nSize, &bytesRead, NULL))
		{
			if (bytesRead == 0)
				menosuno = 1;
			cCrtBuffer = cBuffer;
		}
		else
			throw FILEREADPROBLEM;
	}
	return ((unsigned int)(*(cCrtBuffer++))) & 0x000000FF;
}

void InputStream::close()
{
	closed = 1;
	CloseHandle(hFile);
}