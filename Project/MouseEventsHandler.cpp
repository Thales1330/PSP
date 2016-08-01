#include "MouseEventsHandler.h"

MouseEventsHandler::MouseEventsHandler(Workspace* workspace) : Workspace(workspace)
{
	m_workspace = workspace;
}

MouseEventsHandler::~MouseEventsHandler()
{
}

void MouseEventsHandler::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == 'B' && !m_workspace->m_insertMode) {
	    Bus* newBus = new Bus(m_workspace->m_camera->ScreenToWorld(event.GetPosition()));
	    m_workspace->m_elementList.push_back(newBus);
	    m_workspace->m_insertMode = true;
	    m_workspace->Redraw();
	}
	event.Skip();
}
