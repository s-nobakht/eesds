#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

using namespace std;
class OutputStream
{
private:
	int nSize;
	char *cBuffer;
	char *cCrtBuffer;
	HANDLE hFile;
	int closed;
public:
	OutputStream();
	OutputStream(TCHAR *path, int nSize);
	~OutputStream();
	void write(unsigned int value);
	void close();
};
#endif