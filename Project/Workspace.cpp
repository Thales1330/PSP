#include "Workspace.h"

#include "MouseEventsHandler.h"
#include "Bus.h"

// Camera
Camera::Camera()
{
    m_translation = wxPoint2DDouble(0, 0);
    m_scale = 1.0;
}

Camera::~Camera() {}
wxPoint2DDouble Camera::ScreenToWorld(wxPoint2DDouble screenCoords)
{
    return wxPoint2DDouble(screenCoords.m_x / m_scale - m_translation.m_x,
                           screenCoords.m_y / m_scale - m_translation.m_y);
}

// Workspace
Workspace::Workspace() : WorkspaceBase(NULL)
{
    m_glContext = new wxGLContext(m_glCanvas);
    m_camera = new Camera();
}

Workspace::Workspace(wxWindow* parent, wxString name) : WorkspaceBase(parent)
{
    m_name = name;
    // m_workspaceID = workspaceID;
    m_glContext = new wxGLContext(m_glCanvas);
    m_mouseEventsHandler = new MouseEventsHandler(this);
    m_camera = new Camera();
}

Workspace::~Workspace()
{
    std::vector<Element*>::iterator it = m_elementList.begin();
    while(it != m_elementList.end()) {
	    if(!(*it)) delete *it;
	    it++;
	}

    delete m_camera;
    delete m_mouseEventsHandler;
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
	    element->Draw(m_camera->GetTranslation(), m_camera->GetScale());
	    it++;
	}

    glFlush();
    m_glCanvas->SwapBuffers();
    event.Skip();
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
    if(m_insertMode) m_insertMode = false;
    event.Skip();
}

/*
void Workspace::OnKeyDown(wxKeyEvent& event)
{
    if(event.GetKeyCode() == 'B' && !m_insertMode) {
            Bus* newBus = new Bus(m_camera->ScreenToWorld(event.GetPosition()));
            m_elementList.push_back(newBus);
            m_insertMode = true;
            Redraw();
        }
        event.Skip();
}*/

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
    if(m_insertMode) {
	    std::vector<Element*>::iterator it = m_elementList.end() - 1;
	    Element* element = *it;
	    element->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
	    Redraw();
	}
    event.Skip();
}
