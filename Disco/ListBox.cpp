#include "ListBox.h"

void Form::ListBox::AddItem( char *string, int &value )
{
	Item[ItemNr] = string;
	ItemNr++;
}

void Form::ListBox::AddString( char *string )
{
	Item[ItemNr] = string;
	ItemNr++;
}
