// CertificateManager.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "CertificateManager.h"
#include <Shlwapi.h>

CERTIFICATE_RESULT CheckLicense(LPCTSTR szLicenseFilePath)
{
	if (!PathFileExists(szLicenseFilePath)) {
		return CERT_FILE_NOT_FOUND;
	}

	// ���C�Z���X�t�@�C������

	// �L�������̃`�F�b�N

	// MAC�A�h���X�̃`�F�b�N
	// iphlpapi.h

	return CERT_SUCCESS;
}
