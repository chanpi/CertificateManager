#ifndef __CERTIFICATE_MANAGER__
#define __CERTIFICATE_MANAGER__

#include <Windows.h>

#undef DLL_EXPORT

#ifdef CERTIFICATEMANAGER_EXPORTS
#define DLL_EXPORT		__declspec(dllexport)
#else
#define DLL_EXPORT		__declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	DLL_EXPORT int CheckLicense(LPCTSTR szLicenseFilePath);

#ifdef __cplusplus
}
#endif

#endif /* __CERTIFICATE_MANAGER__ */