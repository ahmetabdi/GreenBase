#ifndef _BUTTON_H
#define _BUTTON_H

#include "Form.h"

class Form::Button
{
public:
	Button();
	Button(float X, float Y, float W, float H, char *string )
	{
		x = X;	y = Y;	w = W;	h = H;
		String = string;
	} // Set the size and location when declaring

	float x;
	float y;
	float w;
	float h;

	bool Visible; // Check if the button is visible
	bool Click; // Check if the button was pressed
	bool Available; // Check if the button is available

	char *String; // The string to display in the middle of the button

	void SetPosition( float X, float Y, float W, float H ); // Set the size and location of the button
	void SetLocation( float X, float Y ); // Set the location of the button
	void SetSize( float W, float H ); // Set the width and height of the button

	void Show( LPDIRECT3DDEVICE9 pDevice ); // Show the Button
	void Hide(); // Visible = false
	void Show(); // Visible = true

private:

	bool hover; // Check if the cursor is above the button
	bool lclick; // 
};

#endif