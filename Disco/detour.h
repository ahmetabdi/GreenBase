#ifndef _CDETOURS_H
#define _CDETOURS_H

#include <windows.h>

class cDetours
{
	public:
		DWORD CreateJMP_EAX( DWORD  SrcVA, DWORD  DstVA, DWORD  Size );
		DWORD CreateJMP_REL( DWORD  SrcVA, DWORD  DstVA, DWORD  Size );
		void * CreateJMP(BYTE *org, CONST BYTE *det, INT len);
		LPVOID CreateJZ(LPVOID lpFuncOrig, LPVOID lpFuncDetour);
		void *memcpy_s( void* pvAddress, const void* pvBuffer, size_t stLen );
	private:
		INT GetDetourLenAuto(PBYTE &pbFuncOrig,INT minDetLen);
};

#endif