// CertificateManager.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "CertificateManager.h"
#include <Shlwapi.h>

CERTIFICATE_RESULT CheckLicense(LPCTSTR szLicenseFilePath)
{
	if (!PathFileExists(szLicenseFilePath)) {
		return CERT_FILE_NOT_FOUND;
	}

	// ライセンスファイルを解凍

	// 有効期限のチェック

	// MACアドレスのチェック
	// iphlpapi.h

	return CERT_SUCCESS;
}
