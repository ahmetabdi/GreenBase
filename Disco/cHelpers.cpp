#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "cHelpers.h"
#include "cPragma.h"
#include "cPragma.h"


using namespace std;

void cHelper::HideDLL(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;
	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h]             
		mov eax, [eax+0Ch]               
		mov dwPEB_LDR_DATA, eax	

		InLoadOrderModuleList:
			mov esi, [eax+0Ch]	     
			mov edx, [eax+10h]	     

		LoopInLoadOrderModuleList: 
		    lodsd		         
			mov esi, eax	
			mov ecx, [eax+18h]  
			cmp ecx, hModule	
			jne SkipA		 
		    mov ebx, [eax]	  
		    mov ecx, [eax+4]  
		    mov [ecx], ebx    
		    mov [ebx+4], ecx	  
			jmp InMemoryOrderModuleList 

		SkipA:
			cmp edx, esi     
			jne LoopInLoadOrderModuleList

		InMemoryOrderModuleList:
			mov eax, dwPEB_LDR_DATA
			mov esi, [eax+14h]
			mov edx, [eax+18h]

		LoopInMemoryOrderModuleList: 
			lodsd
			mov esi, eax
			mov ecx, [eax+10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax] 
			mov ecx, [eax+4]
			mov [ecx], ebx
			mov [ebx+4], ecx
			jmp InInitializationOrderModuleList

		SkipB:
			cmp edx, esi
			jne LoopInMemoryOrderModuleList

		InInitializationOrderModuleList:
			mov eax, dwPEB_LDR_DATA
			mov esi, [eax+1Ch]	  
			mov edx, [eax+20h]	  

		LoopInInitializationOrderModuleList: 
			lodsd
			mov esi, eax		
			mov ecx, [eax+08h]
			cmp ecx, hModule		
			jne SkipC
			mov ebx, [eax] 
			mov ecx, [eax+4]
			mov [ecx], ebx
			mov [ebx+4], ecx
			jmp Finished

		SkipC:
			cmp edx, esi
			jne LoopInInitializationOrderModuleList

		Finished:
			popfd;
			popad;
	}
}

DWORD * cHelper::FindDevice(DWORD Base,DWORD Len)
{
	unsigned long i = 0, n = 0;

	for( i = 0; i < Len; i++ )
	{
        if(*(BYTE *)(Base+i+0x00)==0xC7)n++;
	    if(*(BYTE *)(Base+i+0x01)==0x06)n++;
	    if(*(BYTE *)(Base+i+0x06)==0x89)n++;
	    if(*(BYTE *)(Base+i+0x07)==0x86)n++;	
        if(*(BYTE *)(Base+i+0x0C)==0x89)n++;
	    if(*(BYTE *)(Base+i+0x0D)==0x86)n++;

	    if(n == 6) return (DWORD_PTR *)
			(Base + i + 2);n = 0;
	}
	return(0);
}

CHAR* cHelper::Encrypt(const char* plaintext)
{
	int len = strlen(plaintext);
	char* cyphertext = new char[len+1];
	for(int i=0 ; i<len ; ++i)
	{
		cyphertext[i] = plaintext[i] + 8 - 6;
	}
	cyphertext[len] = 0; 
	return cyphertext;
}

CHAR* cHelper::Decrypt(const char* plaintext)
{
	int len = strlen(plaintext);
	char* cyphertext = new char[len+1];
	for(int i=0 ; i<len ; ++i)
	{
		cyphertext[i] = plaintext[i] - 8 + 6;
	}
	cyphertext[len] = 0; 
	return cyphertext;
}

ofstream ofile;
char dlldir[320];

CHAR * cHelper::GetDirectoryFile(char *filename){
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

VOID __cdecl cHelper::WriteLog (const char *fmt, ...){
	if(ofile != NULL)
	{
	if(!fmt) { return; }
	va_list va_alist;
	char logbuf[255] = {0};
	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);
	ofile << logbuf << endl;
	}
}

VOID cHelper::StarLog(HMODULE hDll)
{
	GetModuleFileName(hDll, dlldir, 512);
	for(int i = strlen(dlldir); i > 0; i--) { if(dlldir[i] == '\\') { dlldir[i+1] = 0; break; } }
	DeleteFile(GetDirectoryFile("[WH]xKarraKa Log.txt"));
	ofile.open(GetDirectoryFile("[WH]xKarraKa Log.txt"), ios::app);
}