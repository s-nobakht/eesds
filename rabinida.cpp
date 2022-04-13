#include "include.h"

RabinIDA::RabinIDA(int n, int m)
{
	this->n = n;
	this->m = m;
	for (int i = 0; i<256; i++)
		memset(mulsInGF256[i], 0, 256 * sizeof(int));
	memset(invsInGF256, 0, 256 * sizeof(int));
	//Compute multiplication table
	for (int a = 0; a<256; a++)
	{
		for (int b = 0; b<256; b++)
		{
			mulsInGF256[a][b] = mulInGF256(a, b);
		}
	}
	//Compute multiplicative inverses
	for (int a = 1; a<256; a++)
	{
		if (invsInGF256[a] == 0)
		{
			for (int b = 1; b<256; b++)
			{
				if (mulsInGF256[a][b] == 1) {
					invsInGF256[a] = b;
					invsInGF256[b] = a;
				}
			}
		}
	}
	//Create the vandermonde vectors
	mat = new unsigned int*[n];
	for (int i = 0; i<n; i++)
	{
		mat[i] = new unsigned int[m];
		memset(mat[i], 0, sizeof(unsigned int)*m);
	}
	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<m; j++)
		{
			mat[i][j] = (j == 0 ? 1 : mulsInGF256[mat[i][j - 1]][i]);
		}
	}
	//
	inv = new unsigned int*[m];
	for (int i = 0; i<m; i++)
	{
		inv[i] = new unsigned int[m];
		memset(inv[i], 0, sizeof(unsigned int)*m);
	}
}

RabinIDA::~RabinIDA()
{
	for (int i = 0; i<n; i++)
		delete[] mat[i];
	delete[] mat;
	for (int i = 0; i<m; i++)
		delete[] inv[i];
	delete[] inv;
}

inline unsigned int RabinIDA::addInGF256(unsigned int a, unsigned int b)
{
	return a^b;
}

unsigned int RabinIDA::mulInGF256(unsigned int a, unsigned int b)
{ // Multiplication in GF(2^8)
	unsigned int p = 0;
	for (; b != 0; b >>= 1)
	{
		p ^= (a*(b & 1));
		a = ((a >> 7) == 1 ? ((a << 1) ^ 0x1B) : (a << 1)) & 0xFF;
	}
	return p;
}

//Inverse of a square Vandermonde Matrix in GF(2^8) using Traub's Algorithm 
//Code from Alejandro Sotelo.
void RabinIDA::vandermondeInverse(unsigned int* e)
{ // Traub's algorithm: O(m^2)
	unsigned int *ak1 = new unsigned int[m + 1];
	unsigned int *ak2 = new unsigned int[m + 1];
	unsigned int *a = ak1;
	memset(ak1, 0, sizeof(unsigned int)*(m + 1));
	memset(ak2, 0, sizeof(unsigned int)*(m + 1));
	ak1[0] = e[0];
	ak1[1] = 1;
	for (int k = 1; k<m; k++)
	{
		for (int i = 0; i <= k + 1; i++)
			ak2[i] = addInGF256(i>0 ? ak1[i - 1] : 0, mulsInGF256[e[k]][i <= k ? ak1[i] : 0]);
		for (int x = 0; x<k + 2; x++)
			ak1[x] = ak2[x];
	}
	for (int j = 0; j<m; j++)
	{
		int p = e[j], q = inv[m - 1][j] = 1, r = a[1];
		for (int k = 1; k<m; k++, p = mulsInGF256[p][e[j]])
		{
			q = inv[m - 1 - k][j] = addInGF256(mulsInGF256[e[j]][q], a[m - k]);
			if ((k & 1) == 0)
				r = addInGF256(r, mulsInGF256[p][a[k + 1]]);
		}
		for (int k = 0; k<m; k++)
			inv[k][j] = mulsInGF256[inv[k][j]][invsInGF256[r]];
	}
	//
	delete[] ak1;
	delete[] ak2;
}

void RabinIDA::split(InputStream *is, OutputStream **os, unsigned long long L)
{
	unsigned int **vec = mat;
	int *arr = new int[n];
	memset(arr, 0, sizeof(int)*n);
	for (int i = 0; i<n; i++)
	{
		os[i]->write(m);
		os[i]->write(m == 1 ? 0 : vec[i][1]);
	}
	for (unsigned long long p = 0; p<L;)
	{
		memset(arr, 0, sizeof(unsigned int)*n);
		for (int j = 0; j<m; j++)
		{
			int v = (p++)<L ? is->read() : 0;
			for (int i = 0; i<n; i++)
				arr[i] = addInGF256(arr[i], mulsInGF256[vec[i][j]][v]);
		}
		for (int i = 0; i<n; i++)
			os[i]->write(arr[i]);
	}
	is->close();
	for (int i = 0; i<n; i++)
		os[i]->close();
	delete[] arr;
}

// Recombine. 1<=m<=n<2^8
void RabinIDA::recombine(InputStream **is, OutputStream *os, unsigned long long L)
{
	unsigned int *e = new unsigned int[m];
	for (int i = 0; i<m; i++)
		if (is[i]->read() != m)
			throw NOTENOUGHFRAGMENTS;
	for (int i = 0; i<m; i++)
		e[i] = is[i]->read();
	vandermondeInverse(e);
	unsigned int *arr = new unsigned int[m];
	memset(arr, 0, sizeof(int)*m);
	for (unsigned long long p = 0; p<L;)
	{
		memset(arr, 0, sizeof(int)*m);
		for (int j = 0; j<m; j++)
		{
			int v = is[j]->read();
			v = v>0 ? v : 0;
			for (int i = 0; i<m; i++)
				arr[i] = addInGF256(arr[i], mulsInGF256[inv[i][j]][v]);
		}
		for (int i = 0; i<m; i++)
			if ((p++)<L)
				os->write(arr[i]);
	}
	os->close();
	for (int i = 0; i<m; i++)
		is[i]->close();
	delete[] e;
	delete[] arr;
}

// Auxiliary method to split files.
void RabinIDA::split(TCHAR* fileIn)
{
	TCHAR tNumber[100];
	TCHAR tBuffer[1 << 8];
	OutputStream **os = new OutputStream*[n];
	InputStream *is = new InputStream(fileIn, 1 << 20);
	for (int i = 0; i<n; i++)
	{
		toString(i, 5, tNumber);
		_stprintf_s(tBuffer, 1 << 8, _T("%s.split%s"), fileIn, tNumber);
		os[i] = new OutputStream(tBuffer, 1 << 20);
	}
	split(is, os, GetFileSize(fileIn));
	delete is;
	for (int i = 0; i<n; i++)
		delete os[i];
	delete[] os;

}

// Auxiliary method to recombine files.
void RabinIDA::recombine(TCHAR* fileSplit, int *idPedazos, TCHAR* fileOut, long long L)
{
	TCHAR tNumber[100];
	TCHAR tBuffer[1 << 8];
	InputStream **is = new InputStream*[m];
	OutputStream *os = new OutputStream(fileOut, 1 << 22);
	for (int i = 0; i<m; i++)
	{
		toString(idPedazos[i], 5, tNumber);
		_stprintf_s(tBuffer, 1 << 8, _T("%s.split%s"), fileSplit, tNumber);
		is[i] = new InputStream(tBuffer, 1 << 20);
	}
	recombine(is, os, L);
	delete[] is;
	delete os;
}

