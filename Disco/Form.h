#ifndef _Form_H
#define _Form_H

#include "Draw.h"

class Form
{
public:
	Form();
	Form(float X, float Y, float W, float H )
	{
		x = X;	y = Y;	w = W;	h = H;
	} // Set the size and location of the form when declaring

	float x;
	float y;
	float w;
	float h;

	bool Minimized; // Check if the form is minimized
	bool Closed; // Check if the form is closed
	bool Focused; // Check if the form is focused
	bool Available; // Check if the form is available

	class Button;//		 /
	class CheckBox;//	 |
	class Slider;//		 |
	class PictureBo;//	 |
	class ListBox;//	 |
	class DropList;//	 > Controls that can be added to the form ^_^
	class Drag;//		 |
	class RadioButton;// /

	char *Title; // The title of the form

	void SetPosition( float X, float Y, float W, float H ); // Set the size and location of the form
	void SetLocation( float X, float Y ); // Set the location of the form
	void SetSize( float W, float H ); // Set the width and height of the form

	void Show( LPDIRECT3DDEVICE9 pDevice );
	void Close();
	void Minimize();

private:

	int order;
	const float bar = 17.f;

	void Move();
};

#endif