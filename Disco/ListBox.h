#ifndef _LISTBOX_H
#define _LISTBOX_H

#include "Form.h"

class Form::ListBox
{
public:
	ListBox();
	ListBox( float X, float Y, float W, float H )
	{
		x = X;	y = Y;	w = W;	h = H;
		ItemNr = 0;
	} // Set the position of the DropList when declaring

	float x;
	float y;
	float w;
	float h;

	int Item; // Current selected item

	int ItemNr; // Total number of items
	char *Item[64]; // Strings of all the items

	bool Visible; // Check if the ListBox is visible
	bool Available; // Check if the ListBox is available

	void AddItem( char *string, int &value ); // Add an item and a variable to the list
	void AddStrings( char *string ); // Add multiple items w/o variables to the list
	void AddString( char *string ); // Add an item w/o a variable to the list

	void SetPosition( float X, float Y, float W, float H ); // Set the location and the size
	void SetLocation( float X, float Y ); // Set the location
	void SetSize( float W, float H ); // Set the width and height

	void Show( LPDIRECT3DDEVICE9 pDevice ); // Show the ListBox
	void Hide(); // Hide the ListBox

private:

	void GetCharacterField(); // Needed when adding multiple string items
};

#endif