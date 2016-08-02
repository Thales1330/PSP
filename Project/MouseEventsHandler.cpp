#include "MouseEventsHandler.h"
#include "Bus.h"

MouseEventsHandler::MouseEventsHandler(Workspace* workspace) : Workspace()
{
	m_workspace = workspace;
}

MouseEventsHandler::~MouseEventsHandler()
{
}

void MouseEventsHandler::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == 'B' && !m_workspace->IsInsertMode()) {
	    Bus* newBus = new Bus(m_workspace->m_camera->ScreenToWorld(event.GetPosition()));
	    m_workspace->GetElementList().push_back(newBus);
	    m_workspace->SetInsertMode();
	    m_workspace->Redraw();
	}
	event.Skip();
}
