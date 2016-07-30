#include "Workspace.h"

Workspace::Workspace(wxWindow* parent, wxString name) : WorkspaceBase(parent)
{
    m_name = name;
    m_glContext = new wxGLContext(m_glCanvas);
}

Workspace::~Workspace()
{
    std::vector<Element*>::iterator it = m_elementList.begin();
    while(it != m_elementList.end()) {
	    if(!(*it)) delete *it;
	    it++;
	}
}

void Workspace::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(m_glCanvas);
    m_glContext->SetCurrent(*m_glCanvas);
    SetViewport();

    // desenhar
	std::vector<Element*>::iterator it = m_elementList.begin();
	while(it != m_elementList.end()) {
	    Element* element = *it;
		element->Draw(wxPoint2DDouble(0,0), 1);
	    it++;
	}

    glFlush();
    m_glCanvas->SwapBuffers();
}

void Workspace::SetViewport()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    double width = m_glCanvas->GetSize().x - 1;
    double height = m_glCanvas->GetSize().y - 1;

    glViewport(0, 0, width, height);  // viewport cobrindo toda a tela

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, height, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void Workspace::OnLeftClickDown(wxMouseEvent& event)
{ 
	Bus* newBus = new Bus(event.GetPosition());
	m_elementList.push_back(newBus);
	Redraw();
}
