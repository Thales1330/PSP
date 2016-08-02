#ifndef MOUSEEVENTSHANDLER_H
#define MOUSEEVENTSHANDLER_H

#include "Workspace.h"

class MouseEventsHandler : public Workspace
{
private:
	Workspace* m_workspace;
public:
	MouseEventsHandler(Workspace* workspace);
	~MouseEventsHandler();
	
	virtual void OnKeyDown(wxKeyEvent& event);

};

#endif // MOUSEEVENTSHANDLER_H
