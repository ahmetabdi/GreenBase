#include "Button.h"

void Form::Button::SetPosition( float X, float Y, float W, float H )
{
	x = X;	y = Y;	w = w;	h = H;
}

void Form::Button::Show( LPDIRECT3DDEVICE9 pDevice )
{
	bool hover = MouseIsOver(x, y, w, h);

	if(hover)
	{
		if(lclick)
		{
			Draw.ThemeRectangle(x, y, w, h, WHITE, pDevice);
			Draw.Text(x+w/2, y+1, String, true, 2, D3DCOLOR_ARGB(255, 220, 220, 220));
		}
		else
		{
			Draw.ThemeRectangle(x, y, w, h, Draw.THEME, pDevice);
			Draw.Text(x+w/2, y+1, String, true, 2, D3DCOLOR_ARGB(255, 230, 230, 230));
			if(l_click) Click = true;
		}
		l_click = click;
	}
	else
	{
		Draw.ThemeRectangle(x, y, w, h, Draw.THEME, pDevice);
		Draw.Text(x+w/2, y+1, msg, true, 2, D3DCOLOR_ARGB(255, 245, 245, 245));
	}

	l_click = Click;
	Click = false;
}