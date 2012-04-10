#ifndef _ZIP_H__
#define _ZIP_H__

#include <string>
using namespace std;

//typedef basic_string<TCHAR> tstring;

#ifdef __cplusplus
extern "C" {
#endif

	//---------------------------------------------------------------------------
	// ZIPƒtƒ@ƒCƒ‹‚ð‰ð“€‚·‚é
	bool Unzip(const string &strZipFilename, const string &strTargetPath);
	bool UnzipCustomizedFile(const string &strZipFilename, char* pfileContents, int length);

#ifdef __cplusplus
}
#endif

#endif //_ZIP_H__
