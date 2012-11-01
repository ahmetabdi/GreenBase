#ifndef _CHACKS_H
#define _CHACKS_H

void *DetourFunc (BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp-len);
}

//void WriteEngine(char *txText, BYTE *Red, BYTE *Blue, BYTE *Green)
//{
//      __asm
//              {
//		push    0x2
//		push    Blue
//		push    Green
//		push    Red
//		push    txText
//		mov     esi, ADR_WRITEINGAME
//		call    esi
//               }
//        Sleep(20);
//}


BOOL bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask) 
			return false;
	return (*szMask) == NULL;
}

DWORD FindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bCompare((BYTE*)(dwAddress+i),bMask,szMask))
			return (DWORD)(dwAddress+i);
	return 0;
}

int HacksInGame(void)
{
	DWORD dwPlayerPointer = *(DWORD*)dwDecypt(ADR_PLAYERBASE);
	if(dwPlayerPointer != 0) 
	{
		//if(Items.norecoil){}
	}

	DWORD dwServerPointer = *(DWORD*)dwDecypt(ADR_SERVERBASE);
	if(dwServerPointer != 0)
	{
		//if(cItem.slot){}
	}
	return 1;
}

#endif