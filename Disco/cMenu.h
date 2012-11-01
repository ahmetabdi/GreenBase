#ifndef _CMENU_H
#define _CMENU_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx9.h>

#ifndef D3DFONT_RIGHT
#define D3DFONT_RIGHT		0x0008
#endif

#define 	MENUMAXITEMS	100
#define 	MENUITEM		1
#define		MENUCAT			2

typedef struct {
  int  typ;		
  char *txt;	
  char **opt;	
  int  *var;	  
  int  maxvalue;
} tMENU;

class Disco
{
public:
	Disco()
	{ 
		MENU=(tMENU **)malloc(4*MENUMAXITEMS);
		for (int i=0; i<MENUMAXITEMS; i++) MENU[i]=(tMENU *)malloc(sizeof(tMENU));
	}
	~Disco()
	{
		for (int i=0; i<MENUMAXITEMS; i++) free(MENU[i]);
		free(MENU);
	}

	bool lclick;
	bool rclick;

	DWORD	CrColor;
	DWORD	CCrColor;
	DWORD	fChams;
	DWORD	bChams;

	int		Mmax;
	int		Mpos;
	int		Cpos;
	float	x_size;  
	float	y_size;	

	//==============================================================================================	
	float MENU_X;//
	float MENU_Y;// Menu
	bool  MENU_V;//
	//==============================================================================================
	tMENU		**MENU;
	//==============================================================================================
	void DragAround(float &MenuX, float &MenuY, float areaX, float areaY, float areaW, float areaH);
	//==============================================================================================
	void AddItem(char *txt, char **opt, int *var, int maxvalue);
	void AddCategory(char *txt, char **opt, int *var, int maxvalue);
	//==============================================================================================
	void MenuShow( short x, short y );
	void ShowMenu();
	void Initialize();
	//==============================================================================================
	//Add your functions here to keep the shit clean
	void DrawCrosshair();
	void Crosshair( D3DCOLOR Color );
	void CircleCrosshair( D3DCOLOR Color );
	//==============================================================================================
	void MenuNav(void);
};


#endif
