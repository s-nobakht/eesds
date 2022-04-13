#include "../include/include.h"

void PrintLastError(TCHAR* lpszFunction)
{
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	_tprintf(_T("ERROR: %s %d\n"), lpMsgBuf, dw);
	LocalFree(lpMsgBuf);
}

long long GetFileSize(const TCHAR *fileName)
{
	long long ret = 0, temp = 0;
	BOOL                        fOk;
	WIN32_FILE_ATTRIBUTE_DATA   fileInfo;
	if (NULL == fileName)
		return -1;
	fOk = GetFileAttributesEx(fileName, GetFileExInfoStandard, (void*)&fileInfo);
	if (!fOk)
		return -1;
	temp = ((long long)(fileInfo.nFileSizeHigh)) >> 32;
	ret |= temp;
	ret |= (long long)fileInfo.nFileSizeLow;
	return ret;
}

void toString(int i, int n, TCHAR *tBuffer)
{
	memset(tBuffer, 0, 50);
	_stprintf_s(tBuffer + (i>9 ? n - 2 : n - 1), 50, _T("%d"), i);
	int it = 0;
	while (tBuffer[it] == 0)
		tBuffer[it++] = '0';
}

