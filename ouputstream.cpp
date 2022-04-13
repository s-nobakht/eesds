#include "include.h"

OutputStream::OutputStream()
{
	cBuffer = NULL;
	closed = 1;
}

OutputStream::OutputStream(TCHAR *tPath, int nSize)
{
	closed = 0;
	this->nSize = nSize;
	cBuffer = new char[nSize];
	cCrtBuffer = cBuffer;
	hFile = CreateFile(tPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		throw FILEOPENPROBLEM;
}

OutputStream::~OutputStream()
{
	if (cBuffer)
		delete[] cBuffer;
	if (!closed)
		close();
}

void OutputStream::write(unsigned int value)
{

	*(cCrtBuffer++) = (unsigned char)value;
	if (cCrtBuffer - cBuffer >= nSize)
	{
		if (!hFile)
			throw FILEWRITEPROBLEM;
		DWORD dwResult = 0;
		if (WriteFile(hFile, cBuffer, nSize, &dwResult, NULL))
			cCrtBuffer = cBuffer;
		else
			throw FILEWRITEPROBLEM;
	}
}

void OutputStream::close()
{
	DWORD dwResult = 0;
	if (cCrtBuffer>cBuffer)
		WriteFile(hFile, cBuffer, cCrtBuffer - cBuffer, &dwResult, NULL);
	closed = 1;
	CloseHandle(hFile);
}