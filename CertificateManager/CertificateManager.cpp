// CertificateManager.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "CertificateManager.h"
#include "SharedConstants.h"
#include "zip.h"
#include "Misc.h"
#include <Shlwapi.h>
#include <IPHlpApi.h>
#include <tchar.h>
#include <string>
using namespace std;

#define BUFFER_SIZE		256

#pragma data_seg(".sharedata")
static int g_result = EXIT_CERT_UNINITIALIZED;
static CHAR g_fileContents[BUFFER_SIZE] = {0};
#pragma data_seg()

static LPCSTR g_szUnconditionalMacAddress = "GG-GG-GG-GG-GG-GG";
static BOOL GetMacAddress(string &strMacAddress, const CHAR split, const CHAR next);

// "20120501/GG-GG-GG-GG-GG-GG"
int CheckLicense(LPCTSTR szLicenseFilePath)
{
	CHAR szZipFileName[BUFFER_SIZE] = {0};
	CHAR szExpireDate[BUFFER_SIZE] = {0};
	CHAR szMacAddress[BUFFER_SIZE] = {0};
	LPSTR p = NULL;

	SYSTEMTIME systemtime = {0};
	CHAR szToday[BUFFER_SIZE] = {0};

	string strMacAddressList;
	CHAR* pszMacAddressList = NULL;
	CHAR* split = "-";
	CHAR* next = "/";
	CHAR* pContext = NULL;	// strtok_s�Ɏg�p

	if (EXIT_CERT_UNINITIALIZED != g_result) {
		return g_result;
	}

	OutputDebugString(_T("---FirstTime---\n"));
	if (!PathFileExists(szLicenseFilePath)) {
		return g_result = EXIT_CERT_FILE_NOT_FOUND;
	}
	
	// ���C�Z���X�t�@�C������
#if UNICODE || _UNICODE
	WideCharToMultiByte(CP_ACP, 0, szLicenseFilePath, _tcslen(szLicenseFilePath), szZipFileName, sizeof(szZipFileName), NULL, NULL);
#else
	strcpy_s(szZipFileName, sizeof(szZipFileName), szLicenseFilePath);
#endif

	UnzipCustomizedFile(szZipFileName, g_fileContents, _countof(g_fileContents));

	// �L��������MAC�A�h���X�ɕ���
	p = StrChrA(g_fileContents, next[0]);
	if (p != NULL) {
		// �L������
		strncpy_s(szExpireDate, _countof(szExpireDate), g_fileContents, p - g_fileContents);
		OutputDebugStringA("Date:[");
		OutputDebugStringA(szExpireDate);
		OutputDebugStringA("]\n");

		// MAC�A�h���X
		strcpy_s(szMacAddress, _countof(szMacAddress), p+1);
		OutputDebugStringA("MAC before:[");
		OutputDebugStringA(szMacAddress);
		OutputDebugStringA("]\n");
		RemoveWhiteSpaceA(szMacAddress);
		OutputDebugStringA("MAC after:[");
		OutputDebugStringA(szMacAddress);
		OutputDebugStringA("]\n");
	}

	// �L�������̃`�F�b�N
	GetLocalTime(&systemtime);
	sprintf_s(szToday, _countof(szToday), "%04u%02u%02u", systemtime.wYear, systemtime.wMonth, systemtime.wDay);

	if (StrCmpA(szToday, szExpireDate) > 0) {
		return g_result = EXIT_CERT_INVALID_EXPIRE_DATE;
	}

	// MAC�A�h���X�̃`�F�b�N
	// ����������
	if (!_stricmp(szMacAddress, g_szUnconditionalMacAddress)) {
		return g_result = EXIT_SUCCESS;
	}

	// ���̃}�V����MAC�A�h���X�̎擾
	if (!GetMacAddress(strMacAddressList, split[0], next[0])) {
		return g_result = EXIT_CERT_SYSTEM_ERROR;
	}

	int len = strMacAddressList.length()+1;
	pszMacAddressList = new CHAR[len];
	if (NULL == pszMacAddressList) {
		return g_result = EXIT_CERT_SYSTEM_ERROR;
	}
	ZeroMemory(pszMacAddressList, sizeof(CHAR) * len);

	if (NULL == StrCpyA(pszMacAddressList, strMacAddressList.c_str())) {
		delete [] pszMacAddressList;
		return g_result = EXIT_CERT_SYSTEM_ERROR;
	}

	// ������MAC�A�h���X���Ԃ�i�ꍇ������ANIC����������Ƃ��j���ߕ���
	CHAR* nextMacAddress = strtok_s(pszMacAddressList, next, &pContext);
	while (NULL != nextMacAddress) {
		if (!_stricmp(szMacAddress, nextMacAddress)) {
			delete [] pszMacAddressList;
			return g_result = EXIT_SUCCESS;
		}
		nextMacAddress = strtok_s(NULL, next, &pContext);
	}

	delete [] pszMacAddressList;
	return g_result = EXIT_CERT_INVALID_MACADDRESS;
}

BOOL GetMacAddress(string &strMacAddress, const CHAR split, const CHAR next)
{
	ULONG uOutBufferLength = 0;
	DWORD dwResult = 0;
	BYTE* pBuffer = NULL;
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	CHAR szTemp[BUFFER_SIZE] = {0};

	// �K�v�Ƃ����o�b�t�@�T�C�Y���擾
	GetAdaptersInfo(NULL, &uOutBufferLength);

	pBuffer = new BYTE[uOutBufferLength];
	if (NULL == pBuffer) {
		OutputDebugString(_T(MESSAGE_ERROR_MEMORY_INVALID));
		return FALSE;
	}
	ZeroMemory(pBuffer, sizeof(BYTE) * uOutBufferLength);
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuffer;
	
	dwResult = GetAdaptersInfo(pAdapterInfo, &uOutBufferLength);
	if (dwResult == ERROR_SUCCESS) {
		for (;;) {
			sprintf_s(szTemp, _countof(szTemp), "%02X%c%02X%c%02X%c%02X%c%02X%c%02X%c",
				pAdapterInfo->Address[0], split,
				pAdapterInfo->Address[1], split,
				pAdapterInfo->Address[2], split,
				pAdapterInfo->Address[3], split,
				pAdapterInfo->Address[4], split,
				pAdapterInfo->Address[5], next);
			strMacAddress.append(szTemp);

			pAdapterInfo = pAdapterInfo->Next;
			if (!pAdapterInfo) {
				break;
			}
		}
	}
	delete [] pBuffer;

	if (strMacAddress.length() > 0) {
		return TRUE;
	}
	return FALSE;
}