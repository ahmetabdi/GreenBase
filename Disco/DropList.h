#ifndef _DROPLIST_H
#define _DROPLIST_H

#include "Form.h"

class Form::DropList
{
public:
	DropList();
	DropList( float X, float Y, float W, float H )
	{
		x = X;	y = Y;	w = W;	h = H;
		ItemNr = 0;
	} // Set the position of the DropList when declaring

	float x;
	float y;
	float w;
	float h;

	int Item; // Current Item

	int ItemNr; // Number of items
	char *Item[64]; // Item strings

	bool Visible; // Check if the DropList can be viewed
	bool Available; // Check if the DropList can be used

	void AddItems( char *string, char delim ); // Add multiple items at a time
	void AddItem( char *string ); // Add an item to the DropList

	void SetPosition( float X, float Y, float W, float H ); // Set the location and the size
	void SetLocation( float X, float Y ); // Set the location
	void SetSize( float W, float H ); // Set the width and height

	void Show( LPDIRECT3DDEVICE9 pDevice ); // Show the DropList
	void Hide(); // Hide the DropList

private:

	void GetCharacterField(); // Needed when adding multiple items
}

#endif