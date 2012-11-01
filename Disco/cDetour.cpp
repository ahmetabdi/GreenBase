#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "ADE32.h"
#include "ADE32.h"
#include "cDetour.h"

void * cDetours::CreateJMP(BYTE *org, const BYTE *det, int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwBack;

	if(len != 0 && len < 5)
		return 0;

	if(!len)
	{
		int tmpLen = 0;
		BYTE *pCurOp = org;
		
		while(tmpLen < 5) {
			int i = oplen(pCurOp);
			if(i == 0 || i == -1)
				return 0;

			tmpLen += i;
			pCurOp += i;
		}

		if(tmpLen < 5)
			return 0;

		len = tmpLen;
	}

	VirtualProtect(org, len, PAGE_READWRITE, &dwBack);
	memcpy(jmp, org, len);	
	jmp += len;
	
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(org+len - jmp) - 5;

	org[0] = 0xE9;
	*(DWORD*)(org+1) = (DWORD)(det - org) - 5;

	for(int i=5; i < len; i++)
		org[i] = 0x90;

	VirtualProtect(org, len, dwBack, &dwBack);
    Sleep(2);
	return (jmp-len);
}

INT cDetours::GetDetourLenAuto(PBYTE &pbFuncOrig,INT minDetLen)
{
    INT len = 0;
    PBYTE pbCurOp = pbFuncOrig;

    while(len < minDetLen)
    {
        int i = oplen(pbCurOp);   
        
        if(i == 0 || i == -1)
            return 0;

        if(len > 64)
            return 0;

        len += i;
        pbCurOp += i;
    }
    return len;
}

LPVOID cDetours::CreateJZ(LPVOID lpFuncOrig, LPVOID lpFuncDetour)
{
    LPVOID lpMallocPtr = NULL;
    PBYTE pbMallocPtr = NULL;
    PBYTE pbFuncOrig = (PBYTE)lpFuncOrig;
    PBYTE pbFuncDetour = (PBYTE)lpFuncDetour;
    PBYTE pbPatchBuf = NULL;
    INT detLen = GetDetourLenAuto(pbFuncOrig,10);
    if((lpMallocPtr = (LPVOID)GlobalAlloc(GPTR,detLen + 5 + 3 )) == NULL)
        return NULL;
    pbMallocPtr = (PBYTE)lpMallocPtr;
    DWORD lpfnOldProtect = NULL,lpNewProtect = PAGE_READWRITE;
    VirtualProtect(lpFuncOrig, detLen,lpNewProtect,&lpfnOldProtect);
    memcpy(lpMallocPtr, lpFuncOrig, detLen);

    pbMallocPtr += detLen;
    pbMallocPtr[0] = 0xE9;
    *(DWORD*)(pbMallocPtr+1) = (DWORD)((pbFuncOrig+detLen)-pbMallocPtr) - 5;
    pbMallocPtr += 0x05;
    pbMallocPtr[0] = 0xCC;
    pbMallocPtr[1] = 0x90;
    pbMallocPtr[2] = 0xC3;
    pbPatchBuf = (BYTE *)GlobalAlloc(GPTR,detLen);
    memset(pbPatchBuf, 0x90, detLen);
    pbPatchBuf[0] = 0x33;
    pbPatchBuf[1] = 0xC0;
    pbPatchBuf[2] = 0x85;
    pbPatchBuf[3] = 0xC0;
    pbPatchBuf[4] = 0x0F;
    pbPatchBuf[5] = 0x84;
    *(DWORD*)&pbPatchBuf[6] = (DWORD)(pbFuncDetour - pbFuncOrig) - 10;
	int i;
    for(i = 0; i < detLen; i++)
          pbFuncOrig[i] = pbPatchBuf[i];
    GlobalFree(pbPatchBuf);
    VirtualProtect(lpFuncOrig, detLen,lpfnOldProtect,&lpNewProtect);
    return lpMallocPtr;
} 

DWORD cDetours::CreateJMP_EAX( DWORD  SrcVA, DWORD  DstVA, DWORD  Size )
{
	DWORD DetourVA, dwProtect, i;
#define SIZEOF_MOVEAX_JMPEAX 7
	if ( SrcVA && DstVA && Size >= SIZEOF_MOVEAX_JMPEAX )
	{
		DetourVA = (DWORD) VirtualAlloc( 
			NULL, Size + SIZEOF_MOVEAX_JMPEAX, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		if ( DetourVA && VirtualProtect( (VOID*)SrcVA, Size, PAGE_EXECUTE_READWRITE, &dwProtect ) )
		{
			for ( i=0; i < Size; i++ ) {
				*(BYTE*)( DetourVA + i ) = *(BYTE*)( SrcVA + i );
			}
			*(BYTE*)( DetourVA + Size + 0 ) = 0xB8;
			*(DWORD*)( DetourVA + Size + 1 ) = ( SrcVA + Size );
			*(WORD*)( DetourVA + Size + 5 ) = 0xE0FF;
			*(BYTE*)( SrcVA + 0 ) = 0xB8;
			*(DWORD*)( SrcVA + 1 ) = ( DstVA );
			*(WORD*)( SrcVA + 5 ) = 0xE0FF;
			VirtualProtect( (VOID*)SrcVA, Size, dwProtect, &dwProtect );
			VirtualProtect( (VOID*)DetourVA, Size + 
				SIZEOF_MOVEAX_JMPEAX, PAGE_EXECUTE_READ, &dwProtect );
	    	return DetourVA;
		}
	}
	return (0);
}

void* cDetours::memcpy_s( void* pvAddress, const void* pvBuffer, size_t stLen )
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery( ( void* )pvAddress, &mbi, sizeof( mbi ) );
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect );
	void* pvRetn = memcpy( ( void* )pvAddress, ( void* )pvBuffer, stLen );
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect );
	FlushInstructionCache( GetCurrentProcess( ), ( void* )pvAddress, stLen );
	return pvRetn;
}

DWORD cDetours::CreateJMP_REL( DWORD  SrcVA, DWORD  DstVA, DWORD  Size )
{
	DWORD DetourVA, dwProtect, i;

#define SIZEOF_JMP_REL  5

	if ( SrcVA && DstVA && Size >= SIZEOF_JMP_REL )
	{
		DetourVA = (DWORD) VirtualAlloc( 
			NULL, Size + SIZEOF_JMP_REL, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE );

		if ( DetourVA && VirtualProtect( (VOID*)SrcVA, Size, PAGE_EXECUTE_READWRITE, &dwProtect ) )
		{
			for ( i=0; i < Size; i++ ) {
				*(BYTE*)( DetourVA + i ) = *(BYTE*)( SrcVA + i );
			}
			
			*(BYTE*)( DetourVA + Size + 0 ) = 0xE9;
			*(DWORD*)( DetourVA + Size + 1 ) = ( SrcVA - DetourVA - SIZEOF_JMP_REL );

	    	        *(BYTE*)( SrcVA + 0 ) = 0xE9;
			*(DWORD*)( SrcVA + 1 ) = ( DstVA - SrcVA - SIZEOF_JMP_REL );

			VirtualProtect( (VOID*)SrcVA, Size, dwProtect, &dwProtect );

			VirtualProtect( (VOID*)DetourVA, Size + 
				SIZEOF_JMP_REL, PAGE_EXECUTE_READ, &dwProtect );

	    	return DetourVA;
		}
	}
	return (0);
}