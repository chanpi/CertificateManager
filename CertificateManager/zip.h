#ifndef _ZIP_H__
#define _ZIP_H__

#include <string>
using namespace std;

//typedef basic_string<TCHAR> tstring;

#ifdef __cplusplug
extern "C" {
#endif

	//---------------------------------------------------------------------------
	// ZIP�t�@�C�����𓀂���
	bool Unzip(const string &strZipFilename, const string &strTargetPath);
	bool UnzipCustomizedFile(const string &strZipFilename, char* pfileContents);

#ifdef __cplusplug
extern "C" {
#endif

#endif //_ZIP_H__
