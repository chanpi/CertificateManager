#include "stdafx.h"

#include <string>
#include <windows.h>
#include <tchar.h>
#include "zip.h"
#include "unzip.h"
using namespace std;

//#pragma comment(lib, "zlib.lib")

#define for if (0) ; else for
#define IsShiftJIS(x) ((BYTE)((x ^ 0x20) - 0xA1) <= 0x3B)

//---------------------------------------------------------------------------
// ファイルが存在するかどうか
bool IsFileExist(const string &strFilename) {
	return GetFileAttributesA(strFilename.c_str()) != 0xffffffff;
}

//---------------------------------------------------------------------------
// 再帰的にディレクトリを作成（strPathの末尾には必ず\をつけること）
bool CreateDirectoryReflex(const string &strPath) {
	const char *p = strPath.c_str();

	while (*p) {
		if (*p == '\\') {
			string strSubPath(strPath.c_str(), p);
			if (!IsFileExist(strSubPath.c_str())) {
				if (!CreateDirectoryA(strSubPath.c_str(), NULL)) {
					return false;
				}
			}
		}
		p += (IsShiftJIS(*p) ? 2 : 1);
	}
	return true;
}

//---------------------------------------------------------------------------
// ZIPファイルを解凍する
bool Unzip(const string &strZipFilename, const string &strTargetPath) {
	unzFile hUnzip = unzOpen(strZipFilename.c_str());
	if (!hUnzip) {
		return false;
	}

	do {
		char szConFilename[512] = {0};
		unz_file_info fileInfo;
		if (unzGetCurrentFileInfo(hUnzip, &fileInfo, szConFilename, sizeof szConFilename, NULL, 0, NULL, 0) != UNZ_OK) {
			break;
		}

		int nLen = strlen(szConFilename);
		int i = 0;
		while (i < nLen) {
			if (szConFilename[i] == '/') {
				szConFilename[i] = '\\';
			}
			i++;
		}

		// ディレクトリの場合
		if (nLen >= 2 && !IsShiftJIS(szConFilename[nLen - 2]) && szConFilename[nLen - 1] == '\\') {
			CreateDirectoryReflex(strTargetPath + szConFilename);
			continue;
		}

		// ファイルの場合
		if (unzOpenCurrentFile(hUnzip) != UNZ_OK) {
			break;
		}

		CreateDirectoryReflex(strTargetPath + szConFilename);
		HANDLE hFile = CreateFileA((strTargetPath + szConFilename).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		BYTE szBuffer[8192] = {0};
		DWORD dwSizeRead = 0;
		while ((dwSizeRead = unzReadCurrentFile(hUnzip, szBuffer, sizeof szBuffer)) > 0) {
			WriteFile(hFile, szBuffer, dwSizeRead, &dwSizeRead, NULL);
		}

		FlushFileBuffers(hFile);
		CloseHandle(hFile);
		unzCloseCurrentFile(hUnzip);

	} while (unzGoToNextFile(hUnzip) != UNZ_END_OF_LIST_OF_FILE);

	unzClose(hUnzip);

	return true;
}

bool UnzipCustomizedFile(const string &strZipFilename, char* pfileContents, int length)
{
	unzFile hUnzip = unzOpen(strZipFilename.c_str());
	if (!hUnzip) {
		return false;
	}

	char szConFilename[512];
	unz_file_info fileInfo;
	if (unzGetCurrentFileInfo(hUnzip, &fileInfo, szConFilename, sizeof szConFilename, NULL, 0, NULL, 0) != UNZ_OK) {
		unzClose(hUnzip);
		return false;
	}

	//int nLen = strlen(szConFilename);
	//for (int i = 0; i < nLen; ++i)
	//	if (szConFilename[i] == '/')
	//	szConFilename[i] = '\\';

	//// ディレクトリの場合
	//if (nLen >= 2 && !IsShiftJIS(szConFilename[nLen - 2]) && szConFilename[nLen - 1] == '\\') {
	//	CreateDirectoryReflex(strTargetPath + szConFilename);
	//	continue;
	//}

	// ファイルの場合
	if (unzOpenCurrentFile(hUnzip) != UNZ_OK) {
		unzClose(hUnzip);
		return false;
	}

	//CreateDirectoryReflex(strTargetPath + szConFilename);
	//HANDLE hFile = CreateFileA((strTargetPath + szConFilename).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	BYTE szBuffer[256] = {0};
	while (unzReadCurrentFile(hUnzip, szBuffer, sizeof szBuffer) > 0) {
		strcpy_s(pfileContents, length, (char*)szBuffer);
	}
	//FlushFileBuffers(hFile);
	//CloseHandle(hFile);

	unzCloseCurrentFile(hUnzip);
	unzClose(hUnzip);

	return true;
}
