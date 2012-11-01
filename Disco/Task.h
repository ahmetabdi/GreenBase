#ifndef _TASK_H
#define _TASK_H

#include "TaskBar.h"
#include "Form.h"

class TaskBar::Task
{
public:

	Task( Form cForm )
	{
		String = &cForm.Title;
		Minimized = &cForm.Minimized;
		Closed = &cForm.Closed;

		TaskBar.TaskNr++;
	}

	void Minimize();
	void Close();

private:

	bool *Minimized;
	bool *Closed;

	char **String;
};

#endif