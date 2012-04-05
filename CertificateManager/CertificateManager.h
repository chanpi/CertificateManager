#ifndef __CERTIFICATE_MANAGER__
#define __CERTIFICATE_MANAGER__

#undef DLL_EXPORT

#ifdef CERTIFICATE_MANAGER
#define DLL_EXPORT		__declspec(dllexport)
#else
#define DLL_EXPORT		__declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	enum CERTIFICATE_RESULT { CERT_SUCCESS,
		CERT_INVALID_MACADDRESS,
		CERT_INVALID_DATE,
		CERT_FILE_NOT_FOUND,
		CERT_SYSTEM_ERROR, };

	DLL_EXPORT CERTIFICATE_RESULT CheckLicense(LPCTSTR szLicenseFilePath);

#ifdef __cplusplus
}
#endif

#endif /* __CERTIFICATE_MANAGER__ */