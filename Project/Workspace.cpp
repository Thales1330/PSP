#include "Workspace.h"

#include "Element.h"
#include "Bus.h"
#include "Line.h"

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
    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

    const int widths[4] = {-3, -1, 100, 100};
    m_statusBar->SetStatusWidths(4, widths);
}

Workspace::~Workspace()
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
	    if(*it) delete *it;
	}
    m_elementList.clear();
    if(m_camera) delete m_camera;
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

    // Elements
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
	    Element* element = *it;
	    element->Draw(m_camera->GetTranslation(), m_camera->GetScale());
	}

    // Selection rectangle
    glColor4d(0.0, 0.5, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(m_selectionRect.m_x, m_selectionRect.m_y);
    glVertex2d(m_selectionRect.m_x, m_selectionRect.m_y + m_selectionRect.m_height);
    glVertex2d(m_selectionRect.m_x + m_selectionRect.m_width, m_selectionRect.m_y + m_selectionRect.m_height);
    glVertex2d(m_selectionRect.m_x + m_selectionRect.m_width, m_selectionRect.m_y);
    glEnd();
    glColor4d(0.0, 0.5, 1.0, 0.3);
    glBegin(GL_QUADS);
    glVertex2d(m_selectionRect.m_x, m_selectionRect.m_y);
    glVertex2d(m_selectionRect.m_x, m_selectionRect.m_y + m_selectionRect.m_height);
    glVertex2d(m_selectionRect.m_x + m_selectionRect.m_width, m_selectionRect.m_y + m_selectionRect.m_height);
    glVertex2d(m_selectionRect.m_x + m_selectionRect.m_width, m_selectionRect.m_y);
    glEnd();

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
    bool foundElement = false;
    if(m_mode == MODE_INSERT) {
	    // Get the last element inserted on the list.
	    Element* newElement = *(m_elementList.end() - 1);
	    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
		    Element* element = *it;
		    // Clicked in any element.
		    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
			    // Click at a bus.
			    if(typeid(*element) == typeid(Bus)) {
				    // Select the bus.
				    element->SetSelected();
				    foundElement = true;  // Element found.
				    // Add the new element's parent. If the element being inserted return true, return
				    // to edit mode.
				    if(newElement->AddParent(element, m_camera->ScreenToWorld(event.GetPosition()))) {
					    m_mode = MODE_EDIT;
					}
				}
			}
		}
	    // The line element can have an indefined number of points.
	    if(!foundElement) {
		    if(typeid(*newElement) == typeid(Line)) {
			    newElement->AddPoint(m_camera->ScreenToWorld(event.GetPosition()));
			}
		}
	    foundElement = true;
	}
    else
	{
	    bool clickPickbox = false;
	    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
		    Element* element = *it;
		    element->ResetPickboxes();  // Reset pickbox state.
		    element->StartMove(m_camera->ScreenToWorld(
		        event.GetPosition()));  // Set movement initial position (not necessarily will be moved).

		    // Click at an element
		    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
			    if(!foundElement) {
				    // Select and show pickbox.
				    element->SetSelected();
				    element->ShowPickbox();
				    foundElement = true;
				}
			    // If pickbox contains the click, move the pickbox
			    if(element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
				    m_mode = MODE_MOVE_PICKBOX;
				    clickPickbox = true;
				}
			    // If didn't foud a pickbox, move the element
			    if(!clickPickbox) {
				    m_mode = MODE_MOVE_ELEMENT;
				}
			}
		}
	}

    if(!foundElement) {
	    m_mode = MODE_SELECTION_RECT;
	    m_startSelRect = m_camera->ScreenToWorld(event.GetPosition());
	}

    Redraw();
    UpdateStatusBar();
    event.Skip();
}

void Workspace::OnRightClickDown(wxMouseEvent& event)
{
    bool redraw = false;
    if(m_mode == MODE_EDIT) {
	    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
		    Element* element = *it;
		    if(element->IsSelected()) {
			    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
				    element->ShowPickbox(false);
				    wxMenu menu;
				    if(element->GetContextMenu(menu)) {
					    menu.SetClientData(element);
					    menu.Connect(wxEVT_COMMAND_MENU_SELECTED,
					                 wxCommandEventHandler(Workspace::OnPopupClick), NULL, this);
					    PopupMenu(&menu);
					}
				    element->ResetPickboxes();
				    redraw = true;
				}
			}
		}
	}
    if(redraw) Redraw();
}

void Workspace::OnLeftClickUp(wxMouseEvent& event)
{
    bool foundPickbox = false;
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
	    Element* element = *it;

	    if(m_mode == MODE_SELECTION_RECT) {
		    if(element->Intersects(m_selectionRect)) {
			    element->SetSelected();
			}
		    else
			{
			    element->SetSelected(false);
			}
		}
	    else
		{
		    // Deselect
		    if(!event.ControlDown()) {
			    if(!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
				    element->SetSelected(false);
				}
			}

		    if(element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
			    foundPickbox = true;
			}
		    else
			{
			    element->ShowPickbox(false);
			}
		}
	}
    if(!foundPickbox) {
	    SetCursor(wxCURSOR_ARROW);
	}

    if(m_mode != MODE_INSERT) {
	    m_mode = MODE_EDIT;
	}
    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);
    Redraw();
    UpdateStatusBar();
}

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
    switch(m_mode)
	{
	    case MODE_INSERT:
		{
		    Element* newElement = *(m_elementList.end() - 1);
		    newElement->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
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
		    bool foundPickbox = false;
		    bool redraw = false;
		    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
			    Element* element = *it;
			    if(element->IsSelected()) {
				    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
					    element->ShowPickbox();
					    redraw = true;

					    if(element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
						    foundPickbox = true;
						    SetCursor(element->GetBestPickboxCursor());
						}
					    else if(!foundPickbox)
						{
						    SetCursor(wxCURSOR_ARROW);
						}
					}
				    else if(!foundPickbox)
					{
					    if(element->IsPickboxShown()) redraw = true;

					    element->ShowPickbox(false);
					    SetCursor(wxCURSOR_ARROW);
					}
				}
			}
		    if(redraw) Redraw();
		}
		break;

	    case MODE_MOVE_PICKBOX:
		{
		    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
			    Element* element = *it;
			    if(element->IsSelected()) {
				    element->MovePickbox(m_camera->ScreenToWorld(event.GetPosition()));
				    Redraw();
				}
			}
		}
		break;

	    case MODE_MOVE_ELEMENT:
		{
		    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
			    Element* element = *it;
			    // Parent's element moving...
			    for(int i = 0; i < (int)element->GetParentList().size(); i++) {
				    Element* parent = element->GetParentList()[i];
				    if(parent->IsSelected()) {
					    element->MoveNode(parent, m_camera->ScreenToWorld(event.GetPosition()));
					}
				}
			    if(element->IsSelected()) {
				    element->Move(m_camera->ScreenToWorld(event.GetPosition()));
				    Redraw();
				}
			}
		}
		break;

	    case MODE_SELECTION_RECT:
		{
		    wxPoint2DDouble currentPos = m_camera->ScreenToWorld(event.GetPosition());
		    double x, y, w, h;
		    if(currentPos.m_x < m_startSelRect.m_x) {
			    x = currentPos.m_x;
			    w = m_startSelRect.m_x - currentPos.m_x;
			}
		    else
			{
			    x = m_startSelRect.m_x;
			    w = currentPos.m_x - m_startSelRect.m_x;
			}
		    if(currentPos.m_y < m_startSelRect.m_y) {
			    y = currentPos.m_y;
			    h = m_startSelRect.m_y - currentPos.m_y;
			}
		    else
			{
			    y = m_startSelRect.m_y;
			    h = currentPos.m_y - m_startSelRect.m_y;
			}

		    m_selectionRect = wxRect2DDouble(x, y, w, h);
		    Redraw();
		}
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
    if(m_mode != MODE_INSERT) {
	    m_mode = MODE_EDIT;
	}
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
			    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
				    Element* element = *it;
				    // Parent's element rotating...
				    for(int i = 0; i < (int)element->GetParentList().size(); i++) {
					    Element* parent = element->GetParentList()[i];
					    if(parent->IsSelected()) {
						    element->RotateNode(parent);
						}
					}
				    if(element->IsSelected()) {
					    element->Rotate();
					}
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
		    case 'L':  // Insert a power line.
			{
			    if(m_mode != MODE_INSERT) {
				    Line* newLine = new Line();
				    m_elementList.push_back(newLine);
				    m_mode = MODE_INSERT;
				    m_statusBar->SetStatusText(_("Insert Line: Click on a bus, ESC to cancel."));
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

	    case MODE_MOVE_ELEMENT:
	    case MODE_MOVE_PICKBOX:
	    case MODE_SELECTION_RECT:
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

void Workspace::OnPopupClick(wxCommandEvent& event)
{
    wxMenu* menu = (wxMenu*)event.GetEventObject();
    Element* element = (Element*)menu->GetClientData();
    switch(event.GetId())
	{
	    case ID_EDIT_BUS:
		{
		    wxMessageBox("Edit bus!");
		}
		break;
	    case ID_EDIT_LINE:
		{
		    wxMessageBox("Edit line!");
		}
		break;
	    case ID_LINE_ADD_NODE:
		{
			Line* line = (Line*)element;
			line->AddNode(m_camera->GetMousePosition());
			Redraw();
		}
		break;
	    case ID_LINE_REMOVE_NODE:
		{
			Line* line = (Line*)element;
			line->RemoveNode(m_camera->GetMousePosition());
			Redraw();
		}
		break;
	    case ID_ROTATE:
		{
		    element->Rotate();
		    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
			    Element* nonEditedElement = *it;
			    // Parent's element rotating...
			    for(int i = 0; i < (int)nonEditedElement->GetParentList().size(); i++) {
				    Element* parent = nonEditedElement->GetParentList()[i];
				    if(parent == element) {
					    nonEditedElement->RotateNode(parent);
					}
				}
			}
		    Redraw();
		}
		break;
	}
}
