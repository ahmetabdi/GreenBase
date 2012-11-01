#ifndef _DROPMENU_H
#define _DROPMENU_H

#include "Form.h"

class DropMenu
{
public:
	DropMenu( Form::Button x )
	{
	}
	DropMenu( Form::CheckBox x )
	{
	}
	DropMenu( Form::DropList x )
	{
	}
	DropMenu( Form::RadioButton x )
	{
	}
	DropMenu( Form::ListBox x )
	{
	}
private:

	void Close();
	void Show();
	void Move();

};

#endif