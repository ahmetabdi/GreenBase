#ifndef _CHELPERS_H
#define _CHELPERS_H

#include <windows.h>
#include <wininet.h>
#include <winsock.h>
#include <urlmon.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "urlmon.lib")

	class cHelper
	{
	public:
		DWORD_PTR * FindDevice(DWORD Base,DWORD Len);
		VOID HideDLL(HINSTANCE hModule);
		VOID StarLog(HMODULE hDll);
		VOID __cdecl cHelper::WriteLog (const char *fmt, ...);
		CHAR* Decrypt(const char* plaintext);
		CHAR* Encrypt(const char* plaintext);
		CHAR * GetTime();
		BOOL CheckVersion();
		INT URLGetPage(char *link, char *buffer, int maxsize);
		BOOL cHelper::_cCompareHost( char *HostName );
	private:
		CHAR * GetDirectoryFile(char *filename);
	};

#endif