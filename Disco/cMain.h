#ifndef _CMAIN_H
#define _CMAIN_H

typedef HRESULT ( WINAPI* oPresent ) ( LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
oPresent pPresent;

typedef HRESULT (WINAPI* oReset) (LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
oReset pReset;

typedef HRESULT (WINAPI* oDrawIndexedPrimitive) (LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
oDrawIndexedPrimitive pDrawIndexedPrimitive;

typedef HRESULT (WINAPI* oSetTransform)(LPDIRECT3DDEVICE9 pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
oSetTransform pSetTransform;

const BYTE bRed[60] =  
{  
    0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,  
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,  
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0xFF, 0x00, 0x00, 0x00  
};
const BYTE bBlue[60] =  
{ 
    0x42, 0x4D, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00,  
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,  
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00,  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 
};

BOOL Create;

LPDIRECT3DTEXTURE9 chams_bcolor, chams_fcolor;
LPDIRECT3DTEXTURE9 texRed, texBlue, texYellow, texGreen, texWhite, texOrange, texBlack, texGray, texPurple; 
LPDIRECT3DTEXTURE9 VISIBLE_CHAMCOLOR;
LPDIRECT3DTEXTURE9 NON_VISIBLE_CHAMCOLOR;

IDirect3DPixelShader9* shaderRed;
IDirect3DPixelShader9* shaderGreen;


LPDIRECT3DVERTEXBUFFER9 StreamData;
UINT OffsetInBytes;
UINT m_Stride;
BOOL Color;
BOOL CHAMS = false;
BOOL RUNONCE = true;

char *Folder[]	= {"", ""};
char *OnOff[]	= {"OFF", "ON"};
char *AimZone[]	= {"Head","Neck","Body","Arms","Feet"};
char *CrossHair[]	= {"OFF","Plus Sign","Circle","Mixed"};
char *CrSize[]		= {"1","2","3","4","5","6","7","8","9","10"};
char *Colors[]		= {"Green","Red","Orange","Yellow","Black","White","Gray","Blue","Purple"};
char *VisibleChamColors[]  = {"White","Red","Orange","Yellow","Black","Green","Gray","Blue","Purple"};
char *NonVisibleChamColors[]  = {"White","Red","Orange","Yellow","Black","Green","Gray","Blue","Purple"};
char *WeaponColor[] = {"OFF","Red","Green","Blue"};

bool m_opt;

D3DXMATRIX cProjection, cWorld, cView ;

#endif