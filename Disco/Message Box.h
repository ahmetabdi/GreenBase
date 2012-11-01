#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Draw.h"

class cMessageBox
{
public:
	cMessageBox();

	char *Title;
	char *Body;

	bool Visible;

	void Show( char *title, char *body );
	void Show();
	void SetTitle( char *string );
	void SetBody( char *string );

private:

	float x;
	float y;
	float w;
	float h;
	void Hide();

} MessageBox;

#endif