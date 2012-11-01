#ifndef _CHECKBOX_H
#define _CHECKBOX_H

#include "Form.h"

class Form::CheckBox
{
public:
	CheckBox();
	CheckBox( float X, float Y )
	{
		x = X;	y = Y;
		Default = 15.f;
	} // Set the location of the CheckBox when declaring

	float x;
	float y;
	float w;
	float h;

	float Default; // The default size of the CheckBox

	bool Visible; // Check if the CheckBox is visible
	bool Checked; // Check if the CheckBox is checked <
	bool Available; // Check if the CheckBox is available

	void SetPosition( float X, float Y, float W, float H ); // Set the size and location of the CheckBox
	void SetLocation( float X, float Y ); // Set the location of the CheckBox
	void SetSize( float W, float H ); // Set the width and height of the CheckBox

	void Show( LPDIRECT3DDEVICE9 pDevice ); // Show the CheckBox
	void Hide(); // Hide the CheckBox

private:

	bool hover; // Check if the cursor is above the CheckBox
	bool click; //
};

#endif