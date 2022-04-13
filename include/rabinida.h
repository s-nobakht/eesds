#ifndef RABINIDA_H
#define RABINIDA_H
class RabinIDA
{
private:
	unsigned int mulsInGF256[256][256];
	unsigned int invsInGF256[256];
	unsigned int **mat;
	int n;
	int m;
	unsigned int **inv;
	// Split. 1<=m<=n<2^8
	void split(InputStream *is, OutputStream **os, unsigned long long L);
	// Recombine. 1<=m<=n<2^8
	void recombine(InputStream **is, OutputStream *os, unsigned long long L);
public:
	RabinIDA(int n, int m);
	~RabinIDA();
	// Adds Galois field GF(2^8)
	inline unsigned int addInGF256(unsigned int a, unsigned int b);
	// Multiplication in the Galois field GF(2^8)
	unsigned int mulInGF256(unsigned int a, unsigned int b);
	// Reverse a mXm Vandermonde matrix with coefficients in GF(2^8),
	// whose first column is the vector of distinct values and [0..m-1]. 1<=m<2^8
	void vandermondeInverse(unsigned int *e);
	// Auxiliary mode to go
	void split(TCHAR* fileIn);
	// Auxiliary mode to recombine
	void recombine(TCHAR* fileSplit, int *idPedazos, TCHAR* fileOut, long long L);

};
#endif