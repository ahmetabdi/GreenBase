#include "Form.h"

void Form::Show( LPDIRECT3DDEVICE9 pDevice )
{
	Draw.Box( x-1, y-1, w+2, h+2, 1, 0, false, BLACK, BLACK, pDevice );
	Draw.Box( x, y, w, h, 1, 0, false, lBACKGROUND, BLACK, pDevice );
	Draw.FillRGBA( x+1, y+1, w-2, bar, lBACKGROUND, pDevice );
	Draw.FillRGBA( x+1, y+1+bar, w-2, h-2-bar, fBACKGROUND, pDevice );
	Draw.Box( x+1, y+1+bar, w-2, h-2-bar, 1, 0, false, BLACK, BLACK, pDevice );

	Available = true;
	Closed = false;
	Minimized = false;
	Focused = true;
}

void Form::Close()
{
	Closed = true;
	Available = false;
	Minimized = false;
	Focused = false;
}

void Form::Minimize()
{
	Minimized = true;
	Closed = false;
	Available = false;
	Focused = false;
}
