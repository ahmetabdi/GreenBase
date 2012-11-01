#ifndef _DRAW_H
#define _DRAW_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define WHITE(a)		D3DCOLOR_ARGB(a	, 255, 255, 255)
#define BLACK(a)		D3DCOLOR_ARGB(a	, 000, 000, 000)
#define GREY(a)			D3DCOLOR_ARGB(a	, 127, 127, 127) 
#define RED(a)			D3DCOLOR_ARGB(a	, 255, 000, 000)
#define BLUE(a)			D3DCOLOR_ARGB(a	, 000, 200, 255)
#define GREEN(a)		D3DCOLOR_ARGB(a	, 127, 255, 000)
#define YELLOW(a)		D3DCOLOR_ARGB(a	, 255, 255, 000)
#define ORANGE(a)		D3DCOLOR_ARGB(a	, 255, 166, 077) 
#define PURPLE(a)		D3DCOLOR_ARGB(a	, 125, 000, 255) 

//Menu Colors
#define MCOLOR_CURRENT	D3DCOLOR_ARGB(255, 000, 200, 255)
#define MCOLOR_TEXT		D3DCOLOR_ARGB(255, 255, 166, 077)
#define MCOLOR_ACTIVE	D3DCOLOR_ARGB(255, 255, 166, 077)
#define MENUCOLOR_TITLE	D3DCOLOR_ARGB(255, 000, 000, 000)
#define MCOLOR_FOLDER	D3DCOLOR_ARGB(255, 255, 167, 076)
#define MCOLOR_INACTIVE	D3DCOLOR_ARGB(255, 255, 166, 077)


#define	left		0
#define centered	1
#define	right		2

struct vertex
{
	float x;
	float y;
	float t;
	float z;
	DWORD Color;
};

struct D3DTLVERTEX
{
	float fX;
	float fY;
	float fZ;
	float fRHW;
	D3DCOLOR Color;
	float fU;
	float fV;
};

D3DTLVERTEX CreateD3DTLVERTEX (float X, float Y, float Z, float RHW, D3DCOLOR color, float U, float V)
{
	D3DTLVERTEX v;

	v.fX = X;
	v.fY = Y;
	v.fZ = Z;
	v.fRHW = RHW;
	v.Color = color;
	v.fU = U;
	v.fV = V;

	return v;
}

class cDraw 
{
public:

	cDraw()
	{
		PI = 3.14159;
		FontNr = 0;
	}
	struct _Screen
	{
		float Width;
		float Height;
		float x_center;
		float y_center;
	} Screen;

	ID3DXFont *pFont[10];

	void FillRGBA( float x1, float y1, float x2, float y2,	float x3, float y3, float x4, float y4,	DWORD c1, DWORD c2, DWORD c3, DWORD c4 );
	void FillRGBA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, DWORD color );
	void FillRGBA( float x, float y, float w, float h, DWORD c1, DWORD c2, DWORD c3, DWORD c4 );
	void FillRGBA( float x, float y, float w, float h, DWORD color );

	void Circle( float x, float y, float rad, bool center, DWORD color );
	void FilledCircle( float x, float y, float rad, bool center, DWORD color );

	void Box( float x, float y, float w, float h, float t, float bt, bool border, DWORD col1, DWORD col2 );
	void CorneredBox( float x, float y, float w, float h, float t, float bt, bool border, DWORD col1, DWORD col2 );

	void Line( float x1, float y1, float x2, float y2, DWORD color );

	void Text( char *text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor );
	
	//=============================================================================================
	bool Font();
	void AddFont( char *Caption, float size, bool bold, bool italic );
	void FontReset();
	void OnLostDevice();
	//=============================================================================================
	void GetDevice( LPDIRECT3DDEVICE9 pDev ) { pDevice = pDev; }
	void Reset();
private:

	LPDIRECT3DDEVICE9 pDevice;
	double PI;
	int FontNr;
};

void cDraw::Reset()
{
	D3DVIEWPORT9 screen;
	pDevice -> GetViewport( &screen );

	Screen.Width = screen.Width;
	Screen.Height = screen.Height;
	Screen.x_center = Screen.Width/2;
	Screen.y_center = Screen.Height/2;
}

void cDraw::Text( char *text, float x, float y, int orientation, int font, bool bordered, DWORD color, DWORD bcolor )
{
	RECT rect;

	switch(orientation)
	{
	case 0:
		if(bordered)
		{
			SetRect( &rect, x - 1, y , x - 1, y );
			pFont[font-1]->DrawTextA(NULL,text,-1,&rect, DT_LEFT|DT_NOCLIP, bcolor );
			SetRect( &rect, x + 1, y , x + 1, y );
			pFont[font-1]->DrawTextA(NULL,text,-1,&rect, DT_LEFT|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y - 1, x, y - 1 );
			pFont[font-1]->DrawTextA(NULL,text,-1,&rect, DT_LEFT|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y + 1, x, y + 1 );
			pFont[font-1]->DrawTextA(NULL,text,-1,&rect, DT_LEFT|DT_NOCLIP, bcolor );
		}
		SetRect( &rect, x, y, x, y );
		pFont[font-1]->DrawTextA(NULL,text,-1,&rect, DT_LEFT|DT_NOCLIP, color );
		break;
	case 1:
		if(bordered)
		{
			SetRect( &rect, x - 1, y , x - 1, y );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_CENTER|DT_NOCLIP, bcolor );
			SetRect( &rect, x + 1, y , x + 1, y );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_CENTER|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y - 1, x, y - 1 );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_CENTER|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y + 1, x, y + 1 );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_CENTER|DT_NOCLIP, bcolor );
		}
		SetRect( &rect, x, y, x, y );
		pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_CENTER|DT_NOCLIP, color );
		break;
	case 2:
		if(bordered)
		{
			SetRect( &rect, x - 1, y , x - 1, y );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_RIGHT|DT_NOCLIP, bcolor );
			SetRect( &rect, x + 1, y , x + 1, y );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_RIGHT|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y - 1, x, y - 1 );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_RIGHT|DT_NOCLIP, bcolor );
			SetRect( &rect, x , y + 1, x, y + 1 );
			pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_RIGHT|DT_NOCLIP, bcolor );
		}
		SetRect( &rect, x, y, x, y );
		pFont[font]->DrawTextA(NULL,text,-1,&rect, DT_RIGHT|DT_NOCLIP, color );
		break;
	}
}

void cDraw::FillRGBA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, DWORD c1, DWORD c2, DWORD c3, DWORD c4 )
{
	vertex V[4];
	
	V[0].Color = c1;
	V[1].Color = c2;
	V[2].Color = c3;
	V[3].Color = c4;

	V[0].z  = V[1].z = V[2].z  = V[3].z  = 0;
	V[0].t  = V[1].t = V[2].t  = V[3].t  = 0;

	V[0].x = x1;	V[0].y = y1;
	V[1].x = x2;	V[1].y = y2;
	V[2].x = x3;	V[2].y = y3;
	V[3].x = x4;	V[3].y = y4;

	pDevice	->	SetTexture			( 0, NULL );
	pDevice	->	SetPixelShader		( NULL );
	pDevice	->	SetRenderState		( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice	->	SetRenderState		(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice	->	SetRenderState		(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice	->	SetFVF				( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	pDevice	->	DrawPrimitiveUP		( D3DPT_TRIANGLESTRIP,2,V,sizeof(vertex));
}

void cDraw::FillRGBA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, DWORD color )
{
	FillRGBA( x1, y1, x2, y2, x3, y3, x4, y4, color, color, color, color );
}

void cDraw::FillRGBA( float x, float y, float w, float h, DWORD c1, DWORD c2, DWORD c3, DWORD c4 )
{
	vertex V[4];
	
	V[0].Color = c1;
	V[1].Color = c2;
	V[2].Color = c3;
	V[3].Color = c4;

	V[0].z  = V[1].z = V[2].z  = V[3].z  = 0;
	V[0].t  = V[1].t = V[2].t  = V[3].t  = 0;

	V[0].x = V[2].x = x;
	V[0].y = V[1].y = y;
	V[1].x = V[3].x = x + w;
	V[2].y = V[3].y = y + h;

	pDevice	->	SetTexture			( 0, NULL );
	pDevice	->	SetPixelShader		( NULL );
	pDevice	->	SetRenderState		( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice	->	SetRenderState		(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice	->	SetRenderState		(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice	->	SetFVF				( D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1 );
	pDevice	->	DrawPrimitiveUP		( D3DPT_TRIANGLESTRIP,2,V,sizeof(vertex));
}

void cDraw::FillRGBA( float x, float y, float w, float h, DWORD color)
{
	FillRGBA( x, y, w, h, color, color, color, color);
}

void cDraw::Circle( float x, float y, float rad, bool center, DWORD color )
{
	const int NUMPOINTS = 34;

	if(!center)
	{
		x -= rad;
		y -= rad;
	}

	D3DTLVERTEX Circle[NUMPOINTS + 1];
	int i;
	float X;
	float Y;
	float Theta;
	float WedgeAngle;
	WedgeAngle = (float)( (2*PI) / NUMPOINTS );
	for( i=0; i<=NUMPOINTS; i++ )
	{
		Theta = i * WedgeAngle;
		X = (float)( x + rad * cos(Theta) );
		Y = (float)( y - rad * sin(Theta) );
		Circle[i] = CreateD3DTLVERTEX( X, Y, 0.0f, 1.0f, color, 0.0f, 0.0f );
	}
	pDevice	->	SetFVF			( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
	pDevice	->	SetTexture		( 0, NULL );
	pDevice	->	DrawPrimitiveUP	( D3DPT_LINESTRIP, NUMPOINTS, &Circle[0], sizeof(Circle[0]) );
}

void cDraw::Box( float x, float y, float w, float h, float t, float bt, bool border, DWORD fcol, DWORD bcol )
{
	if(border)
	{
		FillRGBA(x-t/2-1, y-t/2-1, w+t+2, t+2, bcol );
		FillRGBA(x-t/2-1, y-t/2-1, t+2, h+t+2, bcol );
		FillRGBA(x-t/2-1, y+h-t/2-1, w+t+2, t+2, bcol );
		FillRGBA(x+w-t/2-1, y-t/2-1, t+2, h+t+2, bcol );
	}
	
	FillRGBA(x-t/2, y-t/2, w+t, t, fcol );
	FillRGBA(x-t/2, y-t/2, t, h+t, fcol );
	FillRGBA(x-t/2, y+h-t/2, w+t, t, fcol );
	FillRGBA(x+w-t/2, y-t/2, t, h+t, fcol );
}

void cDraw::Line( float x1, float y1, float x2, float y2, DWORD color )
{
	ID3DXLine *m_Line;

	D3DXCreateLine( pDevice, &m_Line );
	D3DXVECTOR2 line[] = { D3DXVECTOR2( x1, y1 ), D3DXVECTOR2( x2, y2 ) };
	m_Line->Begin();
	m_Line->Draw( line, 2, color );
	m_Line->End();
	m_Line->Release();
}

bool cDraw::Font()
{
	for(int i=0; i<FontNr; i++)
		if(pFont[i]) return false;
	return true;
}

void cDraw::AddFont( char *Caption, float size, bool bold, bool italic )
{
	D3DXCreateFont( pDevice ,
					size,
					0,
					(bold) ? FW_BOLD : FW_NORMAL,
					1, 
					(italic) ? 1 : 0 ,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					ANTIALIASED_QUALITY,
					DEFAULT_PITCH,
					Caption,
					&pFont[FontNr++] );
}

void cDraw::FontReset()
{
	for( int i=0; i<FontNr; i++) pFont[i]->OnLostDevice();
	for( int i=0; i<FontNr; i++) pFont[i]->OnResetDevice();
}

void cDraw::OnLostDevice()
{
	for( int i=0; i<FontNr; i++) pFont[i]->OnLostDevice();
}

#endif