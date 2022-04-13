#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

using namespace std;
class InputStream
{
private:
	int nSize;
	char *cBuffer;
	char *cCrtBuffer;
	HANDLE hFile;
	int closed;
	int menosuno;
	DWORD bytesRead;
public:
	InputStream();
	InputStream(TCHAR *tPath, int nSize);
	~InputStream();
	int read();
	void close();
};
#endif