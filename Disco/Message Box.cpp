#include "Message Box.h"

void cMessageBox::HideMessageBox()
{
	Visible = false;
}

void cMessageBox::SetTitle( char *string )
{
	Title = string;
}

void cMessageBox::SetBody( char *string )
{
	Body = string;
}