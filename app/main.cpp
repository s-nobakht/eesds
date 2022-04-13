/*
==================================================================================
Paper Implementation
Title: "Energy-efficient secure distributed storage in mobile cloud computing"
Authors: Amir Afianian, Saeid Sarabi Nobakht, Mohammad Bagher Ghaznavi-Ghoushchi
Version: 0.9
==================================================================================
*/

#include <iostream>
#include <conio.h>
#include <fstream>
#include <istream>
#include <vector>
#include <cmath>
#include <math.h>
#include "../include/md5.h"
#include "../include/include.h"
#include <time.h>
#include <algorithm>
#include <vector>

using namespace std;

#define HASH_SIZE 16				// md5 hash function produce 16 characters as output
#define NO_SEGMENTS 12				// numbers of segments, equals to "n" in algorithm
#define SEGMENTS_LENGTH 12			// segments length (characters), equals to "m" in algorithm
#define TEST_FILE "sample_file.txt"		// input text file for test
#define PRIMARY_KEY_LENGTH 100		// length of primary key
#define PADDING_CHARACTER 'Z'		// padding character, used to make data size as multiplicat of HASH_SIZE


// forward decleration
std::string cipherKeyGen(std::string &, std::string &);
std::string readField(std::istream, size_t);
std::vector<BYTE> readFile(const char*);
std::vector<char> ReadAllBytes(char const*);

int main()
{
	double encodeTime = 0, decodeTime = 0;
	clock_t tStart = clock();
	clock_t tStop = clock();
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	// reading all contents of file into single string
	std::ifstream ifs(TEST_FILE);
	std::string fileContent((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	std::vector<char> fileCon = ReadAllBytes(TEST_FILE);


	std::string cipherKey = "";												  // cipher key will be exist in this string
	std::string primaryKey = "";											  // primary key will be exist in this string
	
	// amend number of input file characters to make multiplicant of HASH_SIZE
	int paddingNumber = HASH_SIZE - (fileContent.length() % HASH_SIZE);
	for (int i = 0; i < paddingNumber; ++i)
	{
		fileContent += PADDING_CHARACTER;
	}
		
	// define primary key as bytes length random array (in the paper, this array comes from a selfi image !)	
	for (int i = 0; i < PRIMARY_KEY_LENGTH; i++){
		primaryKey += i + 51;
	}

	// create ciphered key (CD) according to primary key (PK) and input data
	cipherKeyGen(primaryKey, fileContent);
	std::string cipheredData = fileContent;
	// create ciphered data (CD) as result of XORing input data and ciphered key
	for (unsigned int i = 0; i<fileContent.length() + 1; i++)
	{
		cipheredData[i] = fileContent[i] ^ cipherKey[i];
	}


	// calculate padding according to segment's length
	int paddingSize = SEGMENTS_LENGTH - ((PRIMARY_KEY_LENGTH + inputTextValue.size()) % SEGMENTS_LENGTH);
	std::string paddingValue = "";											  // segment filling padding string value
	for (int i = 0; i<paddingSize; i++)
	{
		paddingValue += " ";												  // fill input string wit " " character to match its size with P.K.
	}
	

	int segmentSize = inputTextValue.size() / NO_SEGMENTS;
	std::string masterKey = primaryKey;
	std::string tempSegment = "";
	for (int i = 0; i<NO_SEGMENTS; i++)
	{
		tempSegment = cipheredData.substr(i*segmentSize, PRIMARY_KEY_LENGTH);
		for (int i = 0; i<PRIMARY_KEY_LENGTH; i++)
		{
			masterKey[i] = masterKey[i] ^ tempSegment[i];
		}
	}

	std::string cipheredDataF = cipheredData.append(masterKey).append(paddingValue);

	encodeTime += (double)(clock() - tStart);


	std::ofstream out("output\\encode\\en.output.txt");
	out << cipheredDataF;
	out.close();


	DWORD ini = GetTickCount();
	try
	{
		//---- Splite File Example
		string inputFile = "output\\encode\\en.output.txt";
		TCHAR *param = new TCHAR[inputFile.size() + 1];
		param[inputFile.size()] = 0;
		//As much as we'd love to, we can't use memcpy() because
		//sizeof(TCHAR)==sizeof(char) may not be true:
		std::copy(inputFile.begin(), inputFile.end(), param);
		//RabinIDA rabin=RabinIDA(17,10);

		cout << "Number of Segments : " << 12 << endl;
		long long size = GetFileSize(param);
		cout << "Input File Size : " << size << "KB" << endl;

		tStart = clock();
		RabinIDA rabin = RabinIDA(12, 12);
		rabin.split(param);
		encodeTime += (double)(clock() - tStart);
		cout << "Encoding Time : " << encodeTime << "ms" << endl;


		std::vector<std::string> segements;
		WIN32_FIND_DATA search_data;
		memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
		HANDLE handle = FindFirstFile("temp\\CDF.txt.split*", &search_data);
		std::string tempFolder = "temp\\";
		while(handle != INVALID_HANDLE_VALUE)
		{
		cout<<"\n"<<search_data.cFileName;
		std::ifstream ifss(tempFolder.append(search_data.cFileName));
		std::string segmentValue((std::istreambuf_iterator<char>(ifss)), std::istreambuf_iterator<char>());
		segements.push_back(segmentValue);

		if(FindNextFile(handle, &search_data) == FALSE)
		break;
		}

		//Close the handle after use or memory/resource leak
		FindClose(handle);

		

		tStart = clock();

		//---- Recombine File Example	
		string outputFile = "output\\decode\\de.output.dbg.txt";
		TCHAR *param2 = new TCHAR[outputFile.size() + 1];
		param2[outputFile.size()] = 0;
		std::copy(outputFile.begin(), outputFile.end(), param2);
		size = GetFileSize(param);
		//int f[]={0,2,3,5,6,8,9,11,14,15};		        	// parts of file needed to reconstruct		
		//int f[]={0,1,2,3,4,5,6,7,8,9,10,11,12};			// parts of file needed to reconstruct
		int f[NO_SEGMENTS];
		for (int i = 0; i<NO_SEGMENTS; i++)
			f[i] = i;
		rabin.recombine(param, f, param2, size);			// recombine file
		decodeTime += (double)(clock() - tStart);

		ifstream inFile;
		size_t fsize = 0; // here

		inFile.open("output\\decode\\de.output.dbg.txt", ios::in | ios::binary | ios::ate);
		char* oData = 0;

		inFile.seekg(0, ios::end);		// set the pointer to the end
		fsize = inFile.tellg();			// get the length of the file
		//cout << "Size of file: " << size;
		inFile.seekg(0, ios::beg);		// set the pointer to the beginning

		oData = new char[fsize + 1];		//  for the '\0'
		inFile.read(oData, fsize);
		oData[fsize] = '\0';			// set '\0' 

		std::string decodedData;
		// copy raw character into srting
		for (size_t i = 0; i < fsize; i++)
		{
			decodedData.push_back(oData[i]);
		}


		tStart = clock();
		decodedData = decodedData.substr(0, decodedData.size() - (masterKey.size() + paddingSize));
		cipherKey = cipherKeyGen(primaryKey, decodedData.size());
		for (int i = 0; i<filelen + 1; i++)
		{
			decodedData[i] = decodedData[i] ^ cipherKey[i];
		}
		out.open("output\\decode\\de.output.txt");
		out << decodedData;
		out.close();

		decodeTime += (double)(clock() - tStart);

		cout << "Decode Time : " << decodeTime << endl;
		getch();
		
	}
	catch (int)
	{
		PrintLastError(_T("MAIN CATCH"));
		getch();
	}


	return 0;
}



// this function converts md5 value in hex format to an string format
// for bitwise operations (in our case XOR), with need this translation
// we define function as "inline" to speed up our code
inline std::string md5ToString(std::string &md5InHexString)
{
	char temp = 0;
	std::string output = "";
	for (int i = 0; i<32; i = i + 2)
	{
		temp = temp + md5InHexString[i] * 16 + md5InHexString[i];
		output += temp;
		temp = 0;
	}
	return output;
}


// this function generates "Cipher Key" according to input text length
// we define function as "inline" to speed up our code
inline std::string cipherKeyGen(std::string &primaryKey, std::string &fileContent)
{
	std::string cipherKey = "";	
	int keyLengthCounter = fileContent.length() / HASH_SIZE;
	
	int n = PRIMARY_KEY_LENGTH;
	std::vector<bool> v(n);

	for (int r = 1; r < n; r++)
	{
		std::string hashFeed = "";
		std::fill(v.begin(), v.end() - n + r, true);
		do {			
			for (int i = 0; i < n; ++i) {
				if (v[i]) {					
					hashFeed += primaryKey[i];
				}
			}
			cipherKey += md5ToString(md5(hashFeed));			
			--keyLengthCounter;
			if (keyLengthCounter == 0)
				break;
		} while (std::prev_permutation(v.begin(), v.end()));
		if (keyLengthCounter == 0)
			break;
	}

	
	return cipherKey;

}



std::vector<BYTE> readFile(const char* filename)
{
	// open the file:
	std::ifstream file(filename, std::ios::binary);

	// Stop eating new lines in binary mode!!!
	file.unsetf(std::ios::skipws);

	// get its size:
	std::streampos fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// reserve capacity
	std::vector<BYTE> vec;
	vec.reserve(fileSize);

	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<BYTE>(file),
		std::istream_iterator<BYTE>());

	return vec;
}

std::vector<char> ReadAllBytes(char const* filename)
{
	ifstream ifs(filename, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);

	return result;
}