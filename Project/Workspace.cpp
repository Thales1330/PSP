#include "Workspace.h"

#include "Bus.h"

// Camera
Camera::Camera()
{
    m_translation = wxPoint2DDouble(0, 0);
    m_scale = 1.0;
}

Camera::~Camera() {}
wxPoint2DDouble Camera::ScreenToWorld(wxPoint2DDouble screenCoords) const
{
    return wxPoint2DDouble(screenCoords.m_x / m_scale - m_translation.m_x,
                           screenCoords.m_y / m_scale - m_translation.m_y);
}

void Camera::SetTranslation(wxPoint2DDouble screenPoint)
{
    m_translation = screenPoint / m_scale - m_translationStartPt;
}

void Camera::SetScale(wxPoint2DDouble screenPoint, double delta)
{
    m_translation -= screenPoint * (1.0 - m_scale) / m_scale;

    m_scale += delta;

    // Limits: 5% - 300%
    if(m_scale < 0.05) m_scale = 0.05;
    if(m_scale > 3.0) m_scale = 3.0;

    m_translation += screenPoint * (1.0 - m_scale) / m_scale;
}

wxPoint2DDouble Camera::GetMousePosition(bool worldCoords) const
{
    if(worldCoords) return ScreenToWorld(m_mousePosition);
    return m_mousePosition;
}

// Workspace
Workspace::Workspace() : WorkspaceBase(NULL) {}
Workspace::Workspace(wxWindow* parent, wxString name, wxStatusBar* statusBar) : WorkspaceBase(parent)
{
    m_name = name;
    m_statusBar = statusBar;
    m_glContext = new wxGLContext(m_glCanvas);
    m_camera = new Camera();

    const int widths[4] = {-3, -1, 100, 100};
    m_statusBar->SetStatusWidths(4, widths);
}

Workspace::~Workspace()
{
    std::vector<Element*>::iterator it = m_elementList.begin();
    while(it != m_elementList.end()) {
	    if(!(*it)) delete *it;
	    it++;
	}
    // if(!m_statusBar) delete m_statusBar;
    if(!m_camera) delete m_camera;
}

void Workspace::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(m_glCanvas);
    m_glContext->SetCurrent(*m_glCanvas);
    SetViewport();

    // Set GLCanvas scale and translation.
    glScaled(m_camera->GetScale(), m_camera->GetScale(), 0.0);                          // Scale
    glTranslated(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y, 0.0);  // Translation

    // Draw
    std::vector<Element*>::iterator it = m_elementList.begin();
    while(it != m_elementList.end()) {
	    Element* element = *it;
	    element->Draw(m_camera->GetTranslation(), m_camera->GetScale());
	    it++;
	}

    glFlush();  // Sends all pending information directly to the GPU.
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

    // Viewport fit the screen.
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, height, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Workspace::OnLeftClickDown(wxMouseEvent& event)
{
    if(m_mode == MODE_INSERT) {
	    m_mode = MODE_EDIT;
	}
    else
	{
	    std::vector<Element*>::iterator it = m_elementList.begin();
	    while(it != m_elementList.end()) {
		    Element* element = *it;
		    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
			    element->SetSelected();
				element->ShowPickbox();
			}
		    else if(!event.ControlDown())
			{
			    element->SetSelected(false);
			}
		    it++;
		}
	}
    Redraw();
    UpdateStatusBar();
    event.Skip();
}

void Workspace::OnLeftClickUp(wxMouseEvent& event) {}
void Workspace::OnKeyDown(wxKeyEvent& event)
{
    char key = event.GetUnicodeKey();
    if(key != WXK_NONE) {
	    switch(key)
		{
		    case WXK_ESCAPE:  // Cancel operations.
			{
			    if(m_mode == MODE_INSERT) {
				    m_elementList.pop_back();  // Removes the last element being inserted.
				    m_mode = MODE_EDIT;
				    Redraw();
				}
			}
			break;
		    case 'R':  // Rotate the selected elements.
			{
			    std::vector<Element*>::iterator it = m_elementList.begin();
			    while(it != m_elementList.end()) {
				    Element* element = *it;
				    if(element->IsSelected()) {
					    element->Rotate();
					}
				    it++;
				}
			    Redraw();
			}
			break;
		    case 'B':  // Insert a bus.
			{
			    if(m_mode != MODE_INSERT) {
				    Bus* newBus = new Bus(m_camera->ScreenToWorld(event.GetPosition()));
				    m_elementList.push_back(newBus);
				    m_mode = MODE_INSERT;
				    m_statusBar->SetStatusText(_("Insert Bus: Click to insert, ESC to cancel."));
				    Redraw();
				}
			}
			break;
		    default:
			break;
		}
	}

    UpdateStatusBar();
    event.Skip();
}

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
    switch(m_mode)
	{
	    case MODE_INSERT:
		{
		    std::vector<Element*>::iterator it = m_elementList.end() - 1;
		    Element* element = *it;
		    element->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
		    Redraw();
		}
		break;

	    case MODE_DRAG:
		{
		    m_camera->SetTranslation(event.GetPosition());
		    Redraw();
		}
		break;

	    case MODE_EDIT:
		{
		    std::vector<Element*>::iterator it = m_elementList.begin();
		    while(it != m_elementList.end()) {
			    Element* element = *it;
			    if(element->IsSelected()) {
				    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					    element->ShowPickbox();
					}
				    else
					{
					    element->ShowPickbox(false);
					}
				}
			    it++;
			}
		    Redraw();
		}
		break;

	    default:
		break;
	}
    m_camera->UpdateMousePosition(event.GetPosition());
    UpdateStatusBar();
    event.Skip();
}

void Workspace::OnMiddleDown(wxMouseEvent& event)
{
    if(m_mode != MODE_INSERT) {
	    m_mode = MODE_DRAG;
	    m_camera->StartTranslation(m_camera->ScreenToWorld(event.GetPosition()));
	}
    UpdateStatusBar();
}
void Workspace::OnMiddleUp(wxMouseEvent& event)
{
    m_mode = MODE_EDIT;
    UpdateStatusBar();
}
void Workspace::OnScroll(wxMouseEvent& event)
{
    if(event.GetWheelRotation() > 0)
	m_camera->SetScale(event.GetPosition(), +0.05);
    else
	m_camera->SetScale(event.GetPosition(), -0.05);

    UpdateStatusBar();
    Redraw();
}

void Workspace::UpdateStatusBar()
{
    switch(m_mode)
	{
	    case MODE_DRAG:
		{
		    m_statusBar->SetStatusText(_("MODE: DRAG"), 1);
		}
		break;

	    case MODE_INSERT:
		{
		    m_statusBar->SetStatusText(_("MODE: INSERT"), 1);
		}
		break;

	    case MODE_EDIT:
		{
		    m_statusBar->SetStatusText(wxT(""));
		    m_statusBar->SetStatusText(_("MODE: EDIT"), 1);
		}
		break;
	}

    m_statusBar->SetStatusText(wxString::Format(_("ZOOM: %d%%"), (int)(m_camera->GetScale() * 100.0)), 2);
    m_statusBar->SetStatusText(
        wxString::Format(wxT("X: %.1f  Y: %.1f"), m_camera->GetMousePosition().m_x, m_camera->GetMousePosition().m_y),
        3);
}
