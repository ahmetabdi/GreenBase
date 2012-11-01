#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "cPragma.h"
#include "vTableD3D9.h"
#include "cMain.h"
#include "cAddys.h"
#include "cDetour.h"
#include "cHelpers.h"
#include "cMenu.h"
#include "cOptions.h"
#include "cHacks.h"

#include <time.h>
#include "Draw.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "D3DX9.LIB")
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)


bool LogChecks = true;
char String[100];
bool PresentCheck = true;
bool DIPCheck = true;
bool ResetCheck = true;

DWORD	fChamsz;
DWORD	bChamsz;

cHelper		*cHelpers;
cDetours	*cDetour;
Disco		*Menu = NULL;
cDraw		Draw;

DWORD RetnAddress[255];
int Count                = 0;
int ColorCount            = -1;  
static char			textRate1[50]		= {NULL};
static char			textRate2[50]		= {NULL};
static char			textRate3[50]		= {NULL};
static float		fFps				= NULL;
static float		fLastTickCount		= NULL;
static float		fCurrentTickCount;
static char			cFrameRate[50]		= {NULL};

//TEST
bool method; // bool to choose to enable chams while logging to find the logged texture on player model
bool bHave; // bool check to enable the logger output after the value counter has finished logging
UINT curl=0; // current logged texture(this will be output)
UINT curr=0; // no of textures loaded(a bit fuzzy about this ;p)

UINT stri[50000];
UINT vert[50000];
UINT prim[50000]; // arrays to hold the values of each param

char numvertstr[20];
char stridestr[20];
char primcountstr[20];
char currstr[20];
char curlstr[20]; // to hold character strings for output

static char	dStride[50]		= {NULL};
static char	dVert[50]		= {NULL};
static char	dPrim[50]		= {NULL};
static char	dLog[50]		= {NULL};
//END TEST

bool IsMouseOver( short x, short y, short w, short h)
{
	POINT cpos;
	GetCursorPos(&cpos);
	ScreenToClient(GetForegroundWindow(), &cpos);

	return ((cpos.x > x) && (cpos.x < x + w) && (cpos.y > y) && (cpos.y < y + h)) ? 1 : 0;
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
    if(FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
        return E_FAIL;
    
    WORD colour16 = ((WORD)((colour32>>28)&0xF)<<12)
	            	|(WORD)(((colour32>>20)&0xF)<<8)
	             	|(WORD)(((colour32>>12)&0xF)<<4)
                 	|(WORD)(((colour32>>4)&0xF)<<0);

    D3DLOCKED_RECT d3dlr;    
    (*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
    WORD *pDst16 = (WORD*)d3dlr.pBits;

    for(int xy=0; xy < 8*8; xy++)
        *pDst16++ = colour16;

    (*ppD3Dtex)->UnlockRect(0);

    return S_OK;
}

HRESULT GenerateShader(IDirect3DDevice9 *pD3Ddev, IDirect3DPixelShader9 **pShader, float r, float g, float b )
{
	char szShader[ 256 ];
	ID3DXBuffer *pShaderBuf = NULL;
	sprintf( szShader, "ps_3_0\n" "def c0, %f, %f, %f, %f\nmov oC0,c0", r, g, b, 1.0f );
	D3DXAssembleShader( szShader, sizeof( szShader ), NULL, NULL, 0, &pShaderBuf, NULL );
	if( FAILED( pD3Ddev->CreatePixelShader((const DWORD*)pShaderBuf->GetBufferPointer(), pShader)) )return E_FAIL;
	return S_OK;
}

void SetModelColor(LPDIRECT3DDEVICE9 pDevice, float r, float g, float b, float a, float glowr, float glowg, float glowb, float glowa)
{
    float lightValues[4] = {r, g, b, a};
    float glowValues[4] = {glowr, glowg, glowb, glowa};

    pDevice->SetPixelShaderConstantF(1, lightValues, 1);
    pDevice->SetPixelShaderConstantF(3, glowValues, 1);

}  

void RebuildMenu()
{
	Menu->AddCategory("CROSSHAIR"		, CrossHair , &Items.crsh  , 3);
	if(Items.crsh==1)
	{
		Menu->AddItem(" Size"			,CrSize		,&Items.chsize	,	6);
		Menu->AddItem(" Color"			,Colors		,&Items.crcol	,	9);
	}
	else if(Items.crsh==2)
	{
		Menu->AddItem(" Size"			,CrSize		,&Items.cchsize	,	10);
		Menu->AddItem(" Color"			,Colors		,&Items.ccrcol	,	9);
	}

	//Menu->AddCategory("AIMBOT"		, Folder, &Items.aim		,	2);
	//if(Items.aim>0)
	//{
	//	Menu->AddItem(" Aim Zone"		, AimZone	,&Items.zone	,	5);
	//	Menu->AddItem(" Visibility Check",OnOff,	&Items.thru		,	2);
	//	Menu->AddItem(" Auto Shoot"	,	OnOff, &Items.afire		,	2);
	//}

	Menu->AddCategory("VISUALS"		, Folder		,&Items.visuals	,	2);
	if(Items.visuals)
	{
		Menu->AddItem(" Chams"			,OnOff	,&Items.Chams		,2);
		Menu->AddItem(" Visible Colour",VisibleChamColors,&Items.VisibleChamColor, 9);
		Menu->AddItem(" Non-Visible Colour",NonVisibleChamColors,&Items.NonVisibleChamColor, 9);
		Menu->AddItem(" WallHack"		,OnOff	,&Items.wall,	2);
		Menu->AddItem(" Wireframe"		,OnOff	,&Items.wire,	2);
		Menu->AddItem(" Weapon Colour"		,WeaponColor,&Items.WeaponColor,	4);
		Menu->AddItem(" Full Bright"		,OnOff	,&Items.bright,	2);
	}

	//Menu->AddCategory("REMOVALS"	, Folder, &Items.removals ,	2);
	//if(Items.removals)
	//{
	//	Menu->AddItem(" No Recoil"		,OnOff	,&Items.norecoil	,2);
	//	Menu->AddItem(" No Spread"		,OnOff	,&Items.nospread	,2);
	//	Menu->AddItem(" No Reload"		,OnOff	,&Items.noreload	,2);
	//}

	Menu->AddCategory("MISC"	, Folder, &Items.misc ,	2);
	if(Items.misc)
	{
		Menu->AddItem(" CRC LOGGING"				,OnOff	,&Items.wtw,		2);
		Menu->AddItem(" Time/Date/FPS"	,OnOff	,&Items.infobox,	2);
		Menu->AddItem(" MODEL REC", OnOff, &Items.test, 2);
	}
}

void Disco::Initialize()
{
	static bool run = true;
	if( !run ) return;
	run = false;

	Draw.Reset();
	//==============================================================================================
	MENU_X = 10;//
	MENU_Y = 10;// Menu
	//==============================================================================================
	//==============================================================================================
	fChamsz = fChams = D3DCOLOR_ARGB(140, 0, 255, 0);
	bChamsz = bChams = D3DCOLOR_ARGB(140, 255, 0, 0);
	//==============================================================================================
	Mpos		=	0;			
	Mmax		=	0;
	x_size		=	170;
	y_size		=	20.0f;	
	MENU_V		=	1;
	//==============================================================================================
	//Items.colorPicker = 1;
	//==============================================================================================
	CrColor = CCrColor = WHITE(255);

}

//==============================================================================================
//Crosshair
void Disco::Crosshair( DWORD Color )
{
	//The Cross Crosshair
	//==============================================================================================
	//Adjusting the size

	float size;
	float thickness = 1;
	switch(Items.chsize)
	{
	case 0:
		size = 11; break;
	case 1:
		size = 13; break;
	case 2:
		size = 15; break;
	case 3:
		size = 17; break;
	case 4:
		size = 19; break;
	case 5:
		size = 21; break;
	}
	//==============================================================================================
	//Drawing the crosshair
	Draw.FillRGBA(Draw.Screen.x_center - size/2, Draw.Screen.y_center - thickness/2, size, thickness, Color );
	Draw.FillRGBA(Draw.Screen.x_center - thickness/2, Draw.Screen.y_center - size/2, thickness, size, Color );

}

void Disco::CircleCrosshair( D3DCOLOR Color )
{
	//The Circle Crosshair
	//==============================================================================================
	//Adjusting the size
	float rad;

	switch(Items.cchsize)
	{
	case 0:
		rad = 5; break;
	case 1:
		rad = 10; break;
	case 2:
		rad = 15; break;
	case 3:
		rad = 20; break;
	case 4:
		rad = 25; break;
	case 5:
		rad = 30; break;
	case 6:
		rad = 35; break;
	case 7:
		rad = 40; break;
	case 8:
		rad = 45; break;
	case 9:
		rad = 50; break;
	}
	//==============================================================================================
	Draw.Circle( Draw.Screen.x_center, Draw.Screen.y_center, rad, true, Color );
}

void Disco::DrawCrosshair()
{
	if(Items.crsh==1)
	{
		switch(Items.crcol)
		{
		case 0:
			Crosshair(GREEN(255));
			break;
		case 1:
			Crosshair(RED(255));
			break;
		case 2:
			Crosshair(ORANGE(255));
			break;
		case 3:
			Crosshair(YELLOW(255));
			break;
		case 4:
			Crosshair(BLACK(255));
			break;
		case 5:
			Crosshair(WHITE(255));
			break;
		case 6:
			Crosshair(GREY(255));
			break;
		case 7:
			Crosshair(BLUE(255));
			break;
		case 8:
			Crosshair(PURPLE(255));
			break;
		}
	}
	else if(Items.crsh==2)
	{
		switch(Items.ccrcol)
		{
		case 0:
			CircleCrosshair(GREEN(255));
			break;
		case 1:
			CircleCrosshair(RED(255));
			break;
		case 2:
			CircleCrosshair(ORANGE(255));
			break;
		case 3:
			CircleCrosshair(YELLOW(255));
			break;
		case 4:
			CircleCrosshair(BLACK(255));
			break;
		case 5:
			CircleCrosshair(WHITE(255));
			break;
		case 6:
			CircleCrosshair(GREY(255));
			break;
		case 7:
			CircleCrosshair(BLUE(255));
			break;
		case 8:
			CircleCrosshair(PURPLE(255));
			break;
		}
	}
}

//==============================================================================================

HRESULT WINAPI myPresent ( LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	POINT cpos;
	GetCursorPos(&cpos);
	ScreenToClient(GetForegroundWindow(),&cpos);	// Coords Relative to Foreground Windows

	Draw.GetDevice(pDevice);
	Draw.Reset();

	if(LogChecks)PresentCheck = false;

	if( !Create )
	{
		Draw.AddFont( "MS Reference Sans Serif", 15, false, false ); // <======= ADD YOUR FONTS HERE NIGGAH
		Draw.AddFont( "Microsoft Sans Serif",	 15, false, false ); // font 2
		Draw.AddFont( "Visitor TT2",			 12, false, false ); // font 3
		Create = true;
	}
	
	if( Draw.Font() )
		Draw.OnLostDevice();
	
	if(Menu == NULL)
	{
		Menu = new Disco();
	}
	else
	{
		if(Menu->Mmax==0) RebuildMenu();
			
		Menu->DrawCrosshair();
		Menu->MenuShow(Menu->MENU_X,Menu->MENU_Y);
		Menu->MenuNav();
		//WATERMARK
		Draw.Text( "greenV1", 1, 1, left, 1, true, GREEN(255), BLACK(255) );

		if(Items.infobox)
		{
			fCurrentTickCount = clock() * 0.001f;
			++fFps;
			if((fCurrentTickCount - fLastTickCount) > 1.0f)
			{
				fLastTickCount = fCurrentTickCount;
				sprintf( cFrameRate, "[ %d ]", int( fFps ) );
				fFps = 0;
			}
			Draw.Text( cFrameRate, 50, 1, left, 1, true, GREEN(255), BLACK(255) );
		}

		if(Items.wtw)
		{
			sprintf( textRate1, "[ Return Address's Found: %d ]", int( Count ) );
			sprintf( textRate2, "[ Return Address Being Colored: %d ]", int( ColorCount ) );
			sprintf( textRate3, "[ Return Address Being Colored: 0x%.8X ]", RetnAddress[ColorCount] );
			Draw.Text( textRate1, 10, 32, left, 1, true, GREEN(255), BLACK(255) );
			Draw.Text( textRate2, 10, 44, left, 1, true, GREEN(255), BLACK(255) );
			Draw.Text( textRate3, 10, 56, left, 1, true, GREEN(255), BLACK(255) );
		}

		if(Items.test)
		{
			sprintf( dStride, "[ Stride[%d] -> %d ]", curl,stri[curl] );
			sprintf( dVert, "[ NumVertices[%d] -> %d ]", curl, vert[curl] );
			sprintf( dPrim, "[ PrimitiveCount[%d] -> %d ]", curl, prim[curl] );
			sprintf( dLog, "[ Number of Textures logged -> %d ]", curr );
		
			Draw.Text( dStride, 10, 32, left, 1, true, GREEN(255), BLACK(255) );
			Draw.Text( dVert, 10, 44, left, 1, true, GREEN(255), BLACK(255) );
			Draw.Text( dPrim, 10, 56, left, 1, true, GREEN(255), BLACK(255) );
			Draw.Text( dLog, 10, 68, left, 1, true, GREEN(255), BLACK(255) );
		}

	}

	return pPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT WINAPI myDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	
	//CRC LOGGING
	if(Items.wtw)
	{
		if( GetAsyncKeyState( VK_PRIOR )&1 )
		{
			if( Count > 0 )
			{
				ColorCount += 1;
			}
		}
		else if( GetAsyncKeyState( VK_NEXT )&1 )
		{
			if( Count > 0 )
			{
				ColorCount -= 1;
			}
		}

		if( ColorCount > -1 )
		{
			if( RetnAddress[ColorCount] == (DWORD)_ReturnAddress( ) )
			{
				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				pDevice->SetPixelShader(shaderRed);
				pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				pDevice->SetPixelShader(shaderRed);
			}
		}

		for( int i=0; i < sizeof(RetnAddress) / sizeof(DWORD); i++ )
		{
			if( RetnAddress[i] == (DWORD)_ReturnAddress( ) )
				return pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		}

		RetnAddress[Count] = (DWORD)_ReturnAddress( );
		Count += 1;  
	}

	if(pDevice->GetStreamSource(0,&StreamData,&OffsetInBytes,&m_Stride) == D3D_OK) 
	{
		StreamData->Release();
	}

	if(RUNONCE)
	{
		//Generate the diffrent colour textures
		GenerateTexture(pDevice, &texOrange,D3DCOLOR_ARGB(255,255,190,0));
		GenerateTexture(pDevice, &texBlack,D3DCOLOR_ARGB(255,0,0,0));
		GenerateTexture(pDevice, &texGray,D3DCOLOR_ARGB(255,190,190,190));
		GenerateTexture(pDevice, &texPurple,D3DCOLOR_ARGB(255,160,32,240));
		GenerateTexture(pDevice, &texYellow,D3DCOLOR_ARGB(255,255,255,0));
		GenerateTexture(pDevice, &texGreen,D3DCOLOR_ARGB(255,0,255,0));
		GenerateTexture(pDevice, &texRed,D3DCOLOR_ARGB(255,255,0,0));
		GenerateTexture(pDevice, &texBlue,D3DCOLOR_ARGB(255,0,0,255));
		GenerateTexture(pDevice, &texWhite,D3DCOLOR_ARGB(255,255,255,255));
		//General Shader Colours
		GenerateShader(pDevice, &shaderRed, 1.0, 0.0f, 0.0f); 
		GenerateShader(pDevice, &shaderGreen, 0.0, 1.0f, 0.0f);
		RUNONCE = false;
	}

	switch(Items.VisibleChamColor)
	{
		case 0:
			VISIBLE_CHAMCOLOR = texWhite;
			break;
		case 1:
			VISIBLE_CHAMCOLOR = texRed;
			break;
		case 2:
			VISIBLE_CHAMCOLOR = texOrange;
			break;
		case 3:
			VISIBLE_CHAMCOLOR = texYellow;
			break;
		case 4:
			VISIBLE_CHAMCOLOR = texBlack;
			break;
		case 5:
			VISIBLE_CHAMCOLOR = texGreen;
			break;
		case 6:
			VISIBLE_CHAMCOLOR = texGray;
			break;
		case 7:
			VISIBLE_CHAMCOLOR = texBlue;
			break;
		case 8:
			VISIBLE_CHAMCOLOR = texPurple;
			break;
	}

	switch(Items.NonVisibleChamColor)
	{
		case 0:
			NON_VISIBLE_CHAMCOLOR = texWhite;
			break;
		case 1:
			NON_VISIBLE_CHAMCOLOR = texRed;
			break;
		case 2:
			NON_VISIBLE_CHAMCOLOR = texOrange;
			break;
		case 3:
			NON_VISIBLE_CHAMCOLOR = texYellow;
			break;
		case 4:
			NON_VISIBLE_CHAMCOLOR = texBlack;
			break;
		case 5:
			NON_VISIBLE_CHAMCOLOR = texGreen;
			break;
		case 6:
			NON_VISIBLE_CHAMCOLOR = texGray;
			break;
		case 7:
			NON_VISIBLE_CHAMCOLOR = texBlue;
			break;
		case 8:
			NON_VISIBLE_CHAMCOLOR = texPurple;
			break;
	}

	if(Items.Chams)
	{ 
		CHAMS = true;  
	}
	else
	{ 
		CHAMS = false; 
	}


	#define WARZ_HUMANS ( m_Stride == 32 )
	//#define WARZ_HUMANS ((NumVertices == 3557 && primCount == 1672) || (NumVertices == 3989 && primCount == 2014) || (NumVertices == 3339 && primCount == 2014) || (NumVertices == 3339 && primCount == 2236) || (NumVertices == 3955 && primCount == 2588) || (NumVertices == 3555) || (NumVertices == 4404 && primCount == 2285) || (NumVertices == 3339 && primCount == 2236) )
	#define WARZ_WEAPONS ((NumVertices == 4932 && primCount == 2567) || (NumVertices == 3379 && primCount == 2224) || (NumVertices == 5178 && primCount == 3303) || (NumVertices == 5748 && primCount == 2785) || (NumVertices == 7867 && primCount == 3908) || (NumVertices == 3853 && primCount == 1948) || (NumVertices == 8498 && primCount == 4168) || (NumVertices == 3689 && primCount == 2080) || (NumVertices == 7159 && primCount == 3257) || (NumVertices == 3925 && primCount == 1934) || (NumVertices == 4968 && primCount == 2502) || (NumVertices == 3883 && primCount == 1735) || (NumVertices == 4793 && primCount == 2753) || (NumVertices == 5508 && primCount == 3100) || (NumVertices == 4851 && primCount == 3142) || (NumVertices == 213 && primCount == 192) || (NumVertices == 1464 && primCount == 852) || (NumVertices == 3579 && primCount == 1976) || (NumVertices == 4847 && primCount == 2502) || (NumVertices == 2416 && primCount == 1489) || (NumVertices == 1222 && primCount == 530) || (NumVertices == 186 && primCount == 106) || (NumVertices == 3759 && primCount == 1976) || (NumVertices == 3877 && primCount == 2066) || (NumVertices == 190 && primCount == 106) || (NumVertices == 6716 && primCount == 3516) || (NumVertices == 7159 && primCount == 3527))


	if(Items.test)
	{

		bool bHave=false;
		for(int i=0;i<50000;i++){ if(stri[i]==m_Stride && vert[i]==NumVertices && prim[i]==primCount){ bHave=true; break; } }
		if(!bHave){
			stri[curr]=m_Stride;
			vert[curr]=NumVertices;
			prim[curr]=primCount;
			curr++;
		}

		if(stri[curl]==m_Stride && vert[curl]==NumVertices && prim[curl]==primCount){
			if(method){ return NULL; }
			else{ 
				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				pDevice->SetPixelShader(shaderRed);
				pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				pDevice->SetPixelShader(shaderRed);
			}
		}

		itoa(vert[curl], numvertstr, 10);
		itoa(stri[curl], stridestr, 10);
		itoa(prim[curl], primcountstr, 10);
		itoa(curr, currstr, 10); // converts the int datatype to a char datatype

		if( GetAsyncKeyState( VK_NUMPAD1 ) & 1 ){ method = !method; }//disable/enable col
		if( GetAsyncKeyState( VK_SUBTRACT ) & 1 ){ if(curl != 0){ curl--; } }//see next/prev tex
		if( GetAsyncKeyState( VK_ADD ) & 1 ){ if(curl < curr-1){ curl++; } }//see next/prev tex
		if( GetAsyncKeyState( VK_NUMPAD9 ) & 1 ){ if(curl != 0){ curl-=100; } }//see next/prev tex 100
		if( GetAsyncKeyState( VK_NUMPAD6 ) & 1 ){ if(curl < curr-1){ curl+=100; } }//see next/prev tex 100

	}

	if(Items.wall)
	{
		if( WARZ_HUMANS )
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, false); 
			pDevice->SetRenderState(D3DRS_LIGHTING, true); 
			if(CHAMS){
				pDevice->SetTexture(0, NON_VISIBLE_CHAMCOLOR);
			}
			pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255,255,0,0)); 
			pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			pDevice->SetRenderState(D3DRS_ZENABLE, true);     
			pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255,0,255,0));  
			if(CHAMS){
				pDevice->SetTexture(0, VISIBLE_CHAMCOLOR);
			}

			//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			//pDevice->SetPixelShader(shaderRed);
			//pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			//pDevice->SetPixelShader(shaderRed);
		}
	}

	if(Items.wire)
	{
		if( WARZ_HUMANS )
		{
			pDevice -> SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			pDevice -> SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

	if(Items.WeaponColor != 0)
	{
		if (WARZ_WEAPONS)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, false);
			if(Items.WeaponColor == 1)
			{
				SetModelColor(pDevice, 255.0f, 0.0f, 0.0f, 255.0f, 255.0f, 0.0f, 0.0f, 255.0f);
			}
			else if(Items.WeaponColor == 2)
			{
				SetModelColor(pDevice, 0.0f, 255.0f, 0.0f, 255.0f, 0.0f, 255.0f, 0.0f, 255.0f);
			}
			else if(Items.WeaponColor == 3)
			{
				SetModelColor(pDevice, 0.0f, 0.0f, 255.0f, 255.0f, 0.0f, 0.0f, 255.0f, 255.0f);
			}
			pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			pDevice->SetRenderState(D3DRS_ZENABLE, true);
		}
	}

	if(Items.bright)
	{
		if( WARZ_HUMANS )
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, false);
			SetModelColor(pDevice, 255.0f, 0.0f, 0.0f, 255.0f, 255.0f, 0.0f, 0.0f, 255.0f);  
			pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			pDevice->SetRenderState(D3DRS_ZENABLE, true);
		}
	}

	return pDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI myReset ( LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if(LogChecks)ResetCheck = false;

	Draw.FontReset();

	return pReset(pDevice, pPresentationParameters);
}

void Disco::MenuShow( short x, short y )
{
	short i, val;
	short offset = 15; //The maximum number of items that fit the sprite :)
	char text[30];	   //Buffer to draw the text
	DWORD color;	   //The color to draw each menu item

	static bool lastclicked = 0;
	static float xoff, yoff;

	Initialize();

	//==============================================================================================
	if(GetAsyncKeyState(VK_LBUTTON)&1) lclick = true;
	else lclick = false;
	if(GetAsyncKeyState(VK_RBUTTON)&1) rclick = true;
	else rclick = false;
	//==============================================================================================
	if (MENU_V)
	{

		for (i=0; i <Mmax ; i++)//Iterate trough all of the menu items
		{
			if	 	( Mpos < 0 )	 Mpos = Mmax - 1; //
			else if	( Mpos >= Mmax ) Mpos = 0;		  //use your mind

			val = *MENU[i]->var ;
			//==============================================================================================
			//Background rectangles
			if ( MENU[i]->typ == MENUITEM )
				Draw.FillRGBA( x, y, x_size, 19, D3DCOLOR_ARGB(80, 230, 230, 230) );

			else if (MENU[i]->typ==MENUCAT)
				Draw.FillRGBA( x, y, x_size, 19, D3DCOLOR_ARGB(120, 255, 255, 255) );
			//==============================================================================================
			//Normal items
			sprintf(text, "%s %s", MENU[i]->txt, (char *)MENU[i]->opt[val]);
			//==============================================================================================
			//Current item (mouse)
			if(IsMouseOver(x, y,x_size,20))
			{
				Draw.FillRGBA( x-1, y, 1, 19, RED(255) );
				Draw.FillRGBA( x+x_size, y, 1, 19, RED(255) );
				Cpos = i;
				sprintf(text, "> %s %s", MENU[i]->txt, (char *)MENU[i]->opt[val]);
			}
			//==============================================================================================
			//Current item (keyboard)
			if( Mpos == i )
			{
				Draw.FillRGBA(x-1, y, 1, 19, BLUE(255) );
				Draw.FillRGBA(x+x_size, y, 1, 19, BLUE(255) );
				color = MCOLOR_CURRENT;
				sprintf ( text, "• %s %s", MENU[i]->txt, (char *)MENU[i]->opt[val]);
			}
			//==============================================================================================
			//Draw teh text
			Draw.Text( text, x+3, y+2, left, 1, true, WHITE(255), BLACK(255) );
			//==============================================================================================
			//Increasing y's value to draw the next menu item
			y += y_size; 
			//==============================================================================================
		}
	}
}

int HookDX9(void)
{	
	HMODULE hD3D9DLL = 0;
     do 
	 {
		 hD3D9DLL = GetModuleHandle("d3d9.dll");
		 Sleep(20);
	 } while(!hD3D9DLL);
	 
	 Sleep(100);
	 
	 int i = 0;
	 
	 DWORD_PTR * VtablePtr = cHelpers->FindDevice((DWORD)hD3D9DLL,0x128000);
	 if(VtablePtr == NULL)
	 {
		 ExitProcess(0);
	 }
	 DWORD_PTR * VTable = 0;
	 *(DWORD_PTR *)&VTable = *(DWORD_PTR *)VtablePtr;

	 pReset = (oReset)cDetour->CreateJMP_REL((DWORD)VTable[RESET], (DWORD)myReset, 12);
	 pPresent = (oPresent)cDetour->CreateJMP_EAX((DWORD)VTable[PRESENT], (DWORD)myPresent, 12);

	 while(1)
	 {
		 if(memcmp((void*)VTable[82],(void*)(PBYTE)"\x8B\xFF",2)== 0)
		 {
			 pDrawIndexedPrimitive = 
				 (oDrawIndexedPrimitive)cDetour->CreateJMP_EAX
				 ((DWORD)VTable[DRAWINDEXEDPRIMITIVE],(DWORD)myDrawIndexedPrimitive, 7);
		 }
		 Sleep(100);
	 }
	 return 0;
}

DWORD WINAPI EnableHacks(LPVOID param)
{
	while(1) 
	{
		HacksInGame();
		Sleep(10);
	}
}

void MainProc()
{	
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)HookDX9,NULL,NULL,NULL);
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)EnableHacks,NULL,NULL,NULL);
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH) 
	{
		cHelpers->HideDLL(hDll);
		CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)MainProc,NULL,NULL,NULL);
		return TRUE;
	}

	return FALSE;
}

void Disco::MenuNav(void)
{
	if (GetAsyncKeyState(VK_INSERT)&1) MENU_V = !MENU_V;	
	if (!MENU_V) return;

	
	static bool lastclicked = 0;
	static float xoff, yoff;

	POINT cpos;
	GetCursorPos(&cpos);
	ScreenToClient(GetForegroundWindow(), &cpos);

	if(GetAsyncKeyState(VK_CONTROL))
	{
		if(GetAsyncKeyState(VK_UP))		MENU_Y-=3;
		if(GetAsyncKeyState(VK_DOWN))	MENU_Y+=3;
		if(GetAsyncKeyState(VK_LEFT))	MENU_X-=3;
		if(GetAsyncKeyState(VK_RIGHT))	MENU_X+=3;
	}

	if(IsMouseOver(MENU_X, MENU_Y+20*Cpos, x_size, 20))
	{
		if(lclick)
		{
			*MENU[Cpos]->var+=1;
			if (MENU[Cpos]->typ==MENUCAT) Mmax=0;
			if( *MENU[Cpos]->var > MENU[Cpos]->maxvalue-1 ) *MENU[Cpos]->var = 0;
		}
		if(rclick)
		{
			*MENU[Cpos]->var-=1;
			if (MENU[Cpos]->typ==MENUCAT) Mmax=0;
			if( *MENU[Cpos]->var < 0 ) *MENU[Cpos]->var = MENU[Cpos]->maxvalue-1;
		}
	}


	if( (lastclicked)? 1 : IsMouseOver(MENU_X, MENU_Y, 178, Mmax*20) && GetAsyncKeyState(VK_LBUTTON) )
	{
		if(!lastclicked)
		{
			lastclicked = 1;
			xoff = cpos.x - MENU_X;
			yoff = cpos.y - MENU_Y;
		}
		if(lastclicked && MENU_X + xoff != cpos.x)
		{
			MENU_X = cpos.x - xoff;
		}
		if(lastclicked && MENU_Y + yoff != cpos.y)
		{
			MENU_Y = cpos.y - yoff;
		}
	}
	
	if ((GetAsyncKeyState(VK_UP)&1)&&(!(GetAsyncKeyState(VK_CONTROL)))) 
	{
		Mpos--;
		if (Mpos<0)  Mpos=Mmax-1;
	} 
	
	else if ((GetAsyncKeyState(VK_DOWN)&1)&&(!(GetAsyncKeyState(VK_CONTROL))))
	{
		Mpos++;
		if (Mpos==Mmax) Mpos=0;
	}

	if(!GetAsyncKeyState(VK_LBUTTON)) lastclicked = 0;

	short dir = 0;
	if ((( GetAsyncKeyState(VK_LEFT )&1 )&&(!(GetAsyncKeyState(VK_CONTROL))) && *MENU[Mpos]->var > 0) ||(m_opt && GetAsyncKeyState(VK_RBUTTON) && *MENU[Mpos]->var > 0)) dir=-1;
	if ((( GetAsyncKeyState(VK_RIGHT)&1 )&&(!(GetAsyncKeyState(VK_CONTROL))) && *MENU[Mpos]->var < (MENU[Mpos]->maxvalue-1)) ||(m_opt && GetAsyncKeyState(VK_LBUTTON)&& *MENU[Mpos]->var < (MENU[Mpos]->maxvalue-1))) dir=1;
	if (dir) 
	{
		*MENU[Mpos]->var += dir;
		if (MENU[Mpos]->typ==MENUCAT) Mmax=0;
	}
}