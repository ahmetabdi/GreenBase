#ifndef _RADIOBUTTON_H
#define _RADIOBUTTON_H

#include "Form.h"

class Form::RadioButton
{
public:
	RadioButton();
	RadioButton( float X, float Y )
	{
		x = X;	y = Y;
	}

	float x;
	float y;
	float d;

	bool Visible;
	bool Checked;
	bool Available;

	void SetPosition( float X, float Y, float W, float H );
	void SetLocation( float X, float Y );
	void SetSize( float D );

	void Show( LPDIRECT3DDEVICE9 pDevice );
	void Hide();

private:

	bool hover;
}

#endif