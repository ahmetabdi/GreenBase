#include <stdio.h>
#include <time.h>
#include "cMenu.h"
#include "cPragma.h"
#include "Draw.h"

bool MouseIsOver(short x, short y, short w, short h)
{
	POINT cpos;
	GetCursorPos(&cpos);
	ScreenToClient(GetForegroundWindow(), &cpos);

	return ((cpos.x > x) && (cpos.x < x + w) && (cpos.y > y) && (cpos.y < y + h)) ? 1 : 0 ;
}

//==============================================================================================

void Disco::AddCategory(char *txt, char **opt, int *var, int maxvalue)
{
  MENU[Mmax]->typ = MENUCAT;
  MENU[Mmax]->txt = txt;
  MENU[Mmax]->opt = opt;
  MENU[Mmax]->var = var;
  MENU[Mmax]->maxvalue = maxvalue;
  Mmax++;
}

void Disco::AddItem(char *txt, char **opt, int *var, int maxvalue)
{
  MENU[Mmax]->typ = MENUITEM;
  MENU[Mmax]->txt = txt;
  MENU[Mmax]->opt = opt;
  MENU[Mmax]->var = var;
  MENU[Mmax]->maxvalue = maxvalue;
  Mmax++;
}

//==============================================================================================
//Functions

char *GetFrameRate()
{
	static float		fFps				= NULL;
	static float		fLastTickCount		= NULL;
	static float		fCurrentTickCount;
	static char			cFrameRate[50]		= {NULL};

	fCurrentTickCount = clock() * 0.001f;
	++fFps;
	if((fCurrentTickCount - fLastTickCount) > 1.0f)
	{
		fLastTickCount = fCurrentTickCount;
		sprintf( cFrameRate, "[ %d ]", int( fFps ) );
		fFps = 0;
	}
	return cFrameRate;
}