#ifndef _TASKBAR_H
#define _TASKBAR_H

#include "Draw.h"

class cTaskBar
{
public:
	cTaskBar()
	{
		bTaskBar = false;
		bStartMenu = false;

		TaskNr = 0;
	}

	bool bTaskBar;
	bool bStartMenu;
	bool Available;

	class Task;

	int TaskNr;

	void Show( LPDIRECT3DDEVICE9 pDevice );
	void Hide();

	typedef struct
	{
		void Show();
		void Hide();
	} StartMenu;

private:

	float x, y;

	const float b_width = 100, b_height = 18;
	const float inter_space = 3;
	float cr_x;

	void TaskBar( LPDIRECT3DDEVICE9 pDevice );
	void StartMenu( LPDIRECT3DDEVICE9 pDevice );

	void Task( LPDIRECT3DDEVICE9 pDevice );
	void s_Task( LPDIRECT3DDEVICE9 pDevice );

} TaskBar;

#endif