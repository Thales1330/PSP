#include "Workspace.h"

#include "Element.h"
//#include "Bus.h"
#include "Line.h"
#include "Transformer.h"
#include "SyncGenerator.h"
#include "IndMotor.h"
#include "SyncMotor.h"
#include "Load.h"
#include "Inductor.h"
#include "Capacitor.h"

#include "Text.h"

#include "PowerFlow.h"

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
    if(m_scale < m_zoomMin) m_scale = m_zoomMin;
    if(m_scale > m_zoomMax) m_scale = m_zoomMax;

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

    for(int i = 0; i < NUM_ELEMENTS; ++i) {
        m_elementNumber[i] = 1;
    }

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

    // Texts
    for(auto it = m_textList.begin(); it != m_textList.end(); ++it) {
        Text* text = *it;
        text->Draw(m_camera->GetTranslation(), m_camera->GetScale());
    }

    // Selection rectangle
    glLineWidth(1.0);
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
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background.
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
    if(m_mode == MODE_INSERT_TEXT) {
        m_mode = MODE_EDIT;
    } else if(m_mode == MODE_INSERT || m_mode == MODE_DRAG_INSERT) {
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
                    // Add the new element's parent. If the element being inserted returns true, back to
                    // edit mode.
                    if(newElement->AddParent(element, m_camera->ScreenToWorld(event.GetPosition()))) {
                        ValidateElementsVoltages();
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
    } else {
        bool clickPickbox = false;
        for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* element = *it;
            element->ResetPickboxes();  // Reset pickbox state.

            // Set movement initial position (not necessarily will be moved).
            element->StartMove(m_camera->ScreenToWorld(event.GetPosition()));

            // Click in selected element node.
            if(element->NodeContains(m_camera->ScreenToWorld(event.GetPosition())) != 0 && element->IsSelected()) {
                m_mode = MODE_MOVE_NODE;
                foundElement = true;
            }

            // Click in an element.
            else if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
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
                // If didn't found a pickbox, move the element
                if(!clickPickbox) {
                    m_mode = MODE_MOVE_ELEMENT;
                }
            }

            // Click in a switch.
            else if(element->SwitchesContains(m_camera->ScreenToWorld(event.GetPosition()))) {
                element->SetOnline(element->IsOnline() ? false : true);
            }
        }

        // Text element
        for(auto it = m_textList.begin(); it != m_textList.end(); it++) {
            Text* text = *it;

            text->StartMove(m_camera->ScreenToWorld(event.GetPosition()));

            if(text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                if(!foundElement) {
                    text->SetSelected();
                    m_mode = MODE_MOVE_ELEMENT;
                    foundElement = true;
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

void Workspace::OnLeftDoubleClick(wxMouseEvent& event)
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;

        // Click in an element.
        if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
            bool elementIsBus = false;
            Bus oldBus;
            if(typeid(*element) == typeid(Bus)) {
                elementIsBus = true;
                oldBus = *(Bus*)element;
            }
            element->ShowForm(this, element);

            // If the edited element is a bus and was changed the nominal voltage, this voltage must be
            // propagated through the lines
            if(elementIsBus) {
                // The voltage was changed
                if(oldBus.GetEletricalData().nominalVoltage != ((Bus*)element)->GetEletricalData().nominalVoltage ||
                   oldBus.GetEletricalData().nominalVoltageUnit !=
                       ((Bus*)element)->GetEletricalData().nominalVoltageUnit) {
                    // Check if the lines has this bus as parent
                    for(auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
                        Element* child = *it;

                        bool elementIsParent = false;
                        if(typeid(*child) == typeid(Line)) {
                            for(int i = 0; i < (int)child->GetParentList().size(); i++) {
                                Element* parent = child->GetParentList()[i];
                                if(parent == element) {
                                    // TODO: Ask the user if he wants to change all
                                    // voltages
                                    ValidateBusesVoltages(element);
                                    elementIsParent = true;
                                }
                            }
                        }
                        if(elementIsParent) break;
                    }
                }
                ValidateElementsVoltages();
            }
        }

        // Click in a switch.
        else if(element->SwitchesContains(m_camera->ScreenToWorld(event.GetPosition()))) {
            element->SetOnline(element->IsOnline() ? false : true);
        }
    }
}

void Workspace::OnRightClickDown(wxMouseEvent& event)
{
    bool redraw = false;
    if(m_mode == MODE_EDIT) {
        for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* element = *it;
            if(element->IsSelected()) {
                // Show context menu.
                if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                    element->ShowPickbox(false);
                    wxMenu menu;
                    if(element->GetContextMenu(menu)) {
                        menu.SetClientData(element);
                        menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Workspace::OnPopupClick), NULL,
                                     this);
                        PopupMenu(&menu);
                        redraw = true;
                    }
                    element->ResetPickboxes();

                    if(redraw) {
                        Redraw();
                        redraw = false;
                    }
                    // If the last element was removed using the menu, we must leave the "search for" to
                    // prevent error.
                    break;
                }
            }
        }
    }
}

void Workspace::OnLeftClickUp(wxMouseEvent& event)
{
    // This event (under certain conditions) deselects the elements and back to edit mode or select the elements using
    // the selection rectangle.
    bool foundPickbox = false;
    bool findNewParent = false;
    bool updateVoltages = false;
    auto itnp = m_elementList.begin();

    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;

        // The user was moving a pickbox.
        if(m_mode == MODE_MOVE_PICKBOX) {
            // Catch only the element that have the pickbox shown.
            if(element->IsPickboxShown()) {
                // If the element is a bus, check if a node is outside.
                if(typeid(*element) == typeid(Bus)) {
                    // Get all the bus children.
                    for(int i = 0; i < (int)m_elementList.size(); i++) {
                        Element* child = m_elementList[i];
                        for(int j = 0; j < (int)child->GetParentList().size(); j++) {
                            Element* parent = child->GetParentList()[j];
                            // The child have a parent that is the element.
                            if(parent == element) {
                                child->UpdateNodes();
                            }
                        }
                    }
                }
            }
        }

        if(m_mode == MODE_SELECTION_RECT) {
            if(element->Intersects(m_selectionRect)) {
                element->SetSelected();
            } else {
                element->SetSelected(false);
            }
        } else if(m_mode == MODE_MOVE_NODE) {
            if(element->IsSelected()) {
                for(int i = 0; i < (int)m_elementList.size(); i++) {
                    Element* parent = m_elementList[i];
                    if(typeid(*parent) == typeid(Bus)) {
                        if(element->SetNodeParent(parent)) {
                            findNewParent = true;
                            itnp = it;
                            element->ResetNodes();
                            break;
                        }
                    }
                }
                // element->ResetNodes();
            }
        } else {
            // Deselect
            if(!event.ControlDown()) {
                if(!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                    element->SetSelected(false);
                }
            }

            if(element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
                foundPickbox = true;
            } else {
                element->ShowPickbox(false);
                element->ResetPickboxes();
            }
        }
    }

    // Text element
    for(auto it = m_textList.begin(); it != m_textList.end(); it++) {
        Text* text = *it;
        if(m_mode == MODE_SELECTION_RECT) {
            if(text->Intersects(m_selectionRect)) {
                text->SetSelected();
            } else {
                text->SetSelected(false);
            }
        } else if(!event.ControlDown()) {
            if(!text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                text->SetSelected(false);
            }
        }
    }

    if(findNewParent) {
        std::rotate(itnp, itnp + 1, m_elementList.end());
        updateVoltages = true;
    }
    if(!foundPickbox) {
        SetCursor(wxCURSOR_ARROW);
    }

    if(m_mode != MODE_INSERT) {
        m_mode = MODE_EDIT;
    }

    if(updateVoltages) {
        ValidateElementsVoltages();
    }
    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);
    Redraw();
    UpdateStatusBar();
}

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
    bool redraw = false;
    switch(m_mode) {
        case MODE_INSERT: {
            Element* newElement = *(m_elementList.end() - 1);  // Get the last element in the list.
            newElement->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
            redraw = true;
        } break;

        case MODE_INSERT_TEXT: {
            Text* newText = *(m_textList.end() - 1);
            newText->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
            redraw = true;
        } break;

        case MODE_DRAG:
        case MODE_DRAG_INSERT: {
            m_camera->SetTranslation(event.GetPosition());
            redraw = true;
        } break;

        case MODE_EDIT: {
            bool foundPickbox = false;
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* element = *it;
                if(element->IsSelected()) {
                    // Show element pickbox (when it has) if the mouse is over the selected object.
                    if(element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                        element->ShowPickbox();
                        redraw = true;

                        // If the mouse is over a pickbox set correct mouse cursor.
                        if(element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
                            foundPickbox = true;
                            SetCursor(element->GetBestPickboxCursor());
                        } else if(!foundPickbox) {
                            SetCursor(wxCURSOR_ARROW);
                            element->ResetPickboxes();
                        }
                    } else if(!foundPickbox) {
                        if(element->IsPickboxShown()) redraw = true;

                        element->ShowPickbox(false);
                        element->ResetPickboxes();
                        SetCursor(wxCURSOR_ARROW);
                    }
                }
            }
        } break;

        case MODE_MOVE_NODE: {
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* element = *it;
                if(element->IsSelected()) {
                    element->MoveNode(NULL, m_camera->ScreenToWorld(event.GetPosition()));
                    redraw = true;
                }
            }
        } break;

        case MODE_MOVE_PICKBOX: {
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* element = *it;
                if(element->IsSelected()) {
                    element->MovePickbox(m_camera->ScreenToWorld(event.GetPosition()));
                    redraw = true;
                }
            }
        } break;

        case MODE_MOVE_ELEMENT: {
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* element = *it;
                // Parent's element moving...
                for(int i = 0; i < (int)element->GetParentList().size(); i++) {
                    Element* parent = element->GetParentList()[i];
                    if(parent) {
                        if(parent->IsSelected()) {
                            element->MoveNode(parent, m_camera->ScreenToWorld(event.GetPosition()));
                        }
                    }
                }
                if(element->IsSelected()) {
                    element->Move(m_camera->ScreenToWorld(event.GetPosition()));
                    redraw = true;
                }
            }
            // Text element motion
            for(auto it = m_textList.begin(); it != m_textList.end(); it++) {
                Text* text = *it;
                if(text->IsSelected()) {
                    text->Move(m_camera->ScreenToWorld(event.GetPosition()));
                    redraw = true;
                }
            }
        } break;

        case MODE_SELECTION_RECT: {
            wxPoint2DDouble currentPos = m_camera->ScreenToWorld(event.GetPosition());
            double x, y, w, h;
            if(currentPos.m_x < m_startSelRect.m_x) {
                x = currentPos.m_x;
                w = m_startSelRect.m_x - currentPos.m_x;
            } else {
                x = m_startSelRect.m_x;
                w = currentPos.m_x - m_startSelRect.m_x;
            }
            if(currentPos.m_y < m_startSelRect.m_y) {
                y = currentPos.m_y;
                h = m_startSelRect.m_y - currentPos.m_y;
            } else {
                y = m_startSelRect.m_y;
                h = currentPos.m_y - m_startSelRect.m_y;
            }

            m_selectionRect = wxRect2DDouble(x, y, w, h);
            redraw = true;
        } break;
    }

    if(redraw) Redraw();
    m_camera->UpdateMousePosition(event.GetPosition());
    UpdateStatusBar();
    event.Skip();
}

void Workspace::OnMiddleDown(wxMouseEvent& event)
{
    // Set to drag mode.
    if(m_mode != MODE_INSERT && m_mode != MODE_DRAG_INSERT) {
        m_mode = MODE_DRAG;
    } else {
        m_mode = MODE_DRAG_INSERT;
    }
    m_camera->StartTranslation(m_camera->ScreenToWorld(event.GetPosition()));
    UpdateStatusBar();
}
void Workspace::OnMiddleUp(wxMouseEvent& event)
{
    if(m_mode != MODE_INSERT && m_mode != MODE_DRAG_INSERT) {
        // Set to edit mode back.
        m_mode = MODE_EDIT;
    } else if(m_mode == MODE_DRAG_INSERT) {
        m_mode = MODE_INSERT;
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
        switch(key) {
            case WXK_ESCAPE:  // Cancel operations.
            {
                if(m_mode == MODE_INSERT) {
                    m_elementList.pop_back();  // Removes the last element being inserted.
                    m_mode = MODE_EDIT;
                    Redraw();
                }
                else if(m_mode == MODE_INSERT_TEXT) {
                    m_textList.pop_back();
                    m_mode = MODE_EDIT;
                    Redraw();
                }
            } break;
            case WXK_DELETE:  // Delete selected elements
            {
                DeleteSelectedElements();
            } break;
            case 'A': {
                Text* newBus = new Text(m_camera->ScreenToWorld(event.GetPosition()));
                m_textList.push_back(newBus);
                m_mode = MODE_INSERT_TEXT;
                m_statusBar->SetStatusText(_("Insert Text: Click to insert, ESC to cancel."));
                Redraw();
            }
            case 'F': {
                if(event.GetModifiers() == wxMOD_SHIFT) {
                    Fit();
                }
            } break;
            case 'R':  // Rotate the selected elements.
            {
                RotateSelectedElements(event.GetModifiers() != wxMOD_SHIFT);
            } break;
            case 'B':  // Insert a bus.
            {
                if(m_mode != MODE_INSERT) {
                    Bus* newBus = new Bus(m_camera->ScreenToWorld(event.GetPosition()),
                                          wxString::Format(_("Bus %d"), GetElementNumber(ID_BUS)));
                    IncrementElementNumber(ID_BUS);
                    m_elementList.push_back(newBus);
                    m_mode = MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Bus: Click to insert, ESC to cancel."));
                    Redraw();
                }
            } break;
            case 'L': {
                if(m_mode != MODE_INSERT) {
                    if(event.GetModifiers() == wxMOD_SHIFT) {  // Insert a load.
                        Load* newLoad = new Load(wxString::Format(_("Load %d"), GetElementNumber(ID_LOAD)));
                        IncrementElementNumber(ID_LOAD);
                        m_elementList.push_back(newLoad);
                        m_mode = MODE_INSERT;
                        m_statusBar->SetStatusText(_("Insert Load: Click on a buses, ESC to cancel."));
                    } else {  // Insert a power line.
                        Line* newLine = new Line(wxString::Format(_("Line %d"), GetElementNumber(ID_LINE)));
                        IncrementElementNumber(ID_LINE);
                        m_elementList.push_back(newLine);
                        m_mode = MODE_INSERT;
                        m_statusBar->SetStatusText(_("Insert Line: Click on two buses, ESC to cancel."));
                    }
                    Redraw();
                }
            } break;
            case 'T':  // Insert a transformer.
            {
                if(m_mode != MODE_INSERT) {
                    Transformer* newTransformer =
                        new Transformer(wxString::Format(_("Transformer %d"), GetElementNumber(ID_TRANSFORMER)));
                    IncrementElementNumber(ID_TRANSFORMER);
                    m_elementList.push_back(newTransformer);
                    m_mode = MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Transformer: Click on two buses, ESC to cancel."));
                    Redraw();
                }
            } break;
            case 'G':  // Insert a generator.
            {
                if(m_mode != MODE_INSERT) {
                    SyncGenerator* newGenerator =
                        new SyncGenerator(wxString::Format(_("Generator %d"), GetElementNumber(ID_SYNCGENERATOR)));
                    IncrementElementNumber(ID_SYNCGENERATOR);
                    m_elementList.push_back(newGenerator);
                    m_mode = MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Generator: Click on a buses, ESC to cancel."));
                    Redraw();
                }
            } break;
            case 'I': {
                if(m_mode != MODE_INSERT) {
                    if(event.GetModifiers() == wxMOD_SHIFT) {  // Insert an inductor.
                        Inductor* newInductor =
                            new Inductor(wxString::Format(_("Inductor %d"), GetElementNumber(ID_INDUCTOR)));
                        IncrementElementNumber(ID_INDUCTOR);
                        m_elementList.push_back(newInductor);
                        m_mode = MODE_INSERT;
                        m_statusBar->SetStatusText(_("Insert Inductor: Click on a buses, ESC to cancel."));
                    } else  // Insert an induction motor.
                    {
                        IndMotor* newIndMotor =
                            new IndMotor(wxString::Format(_("Induction motor %d"), GetElementNumber(ID_INDMOTOR)));
                        IncrementElementNumber(ID_INDMOTOR);
                        m_elementList.push_back(newIndMotor);
                        m_mode = MODE_INSERT;
                        m_statusBar->SetStatusText(_("Insert Induction Motor: Click on a buses, ESC to cancel."));
                    }
                    Redraw();
                }
            } break;
            case 'K':  // Insert a synchronous condenser.
            {
                if(m_mode != MODE_INSERT) {
                    SyncMotor* newSyncCondenser =
                        new SyncMotor(wxString::Format(_("Synchronous condenser %d"), GetElementNumber(ID_SYNCMOTOR)));
                    IncrementElementNumber(ID_SYNCMOTOR);
                    m_elementList.push_back(newSyncCondenser);
                    m_mode = MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Synchronous Condenser: Click on a buses, ESC to cancel."));
                    Redraw();
                }
            } break;
            case 'C': {
                if(m_mode != MODE_INSERT) {
                    if(event.GetModifiers() == wxMOD_SHIFT) {  // Insert a capacitor.
                        Capacitor* newCapacitor =
                            new Capacitor(wxString::Format(_("Capacitor %d"), GetElementNumber(ID_CAPACITOR)));
                        IncrementElementNumber(ID_CAPACITOR);
                        m_elementList.push_back(newCapacitor);
                        m_mode = MODE_INSERT;
                        m_statusBar->SetStatusText(_("Insert Capacitor: Click on a buses, ESC to cancel."));
                        Redraw();
                    }
                }
            } break;
            default:
                break;
        }
    }

    UpdateStatusBar();
    event.Skip();
}

void Workspace::UpdateStatusBar()
{
    switch(m_mode) {
        case MODE_DRAG: {
            m_statusBar->SetStatusText(_("MODE: DRAG"), 1);
        } break;

        case MODE_INSERT:
        case MODE_INSERT_TEXT:
        case MODE_DRAG_INSERT: {
            m_statusBar->SetStatusText(_("MODE: INSERT"), 1);
        } break;

        case MODE_MOVE_ELEMENT:
        case MODE_MOVE_PICKBOX:
        case MODE_MOVE_NODE:
        case MODE_SELECTION_RECT:
        case MODE_EDIT: {
            m_statusBar->SetStatusText(wxT(""));
            m_statusBar->SetStatusText(_("MODE: EDIT"), 1);
        } break;
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
    switch(event.GetId()) {
        case ID_EDIT_BUS: {
            wxMessageBox("Edit bus!");
        } break;
        case ID_EDIT_LINE: {
            wxMessageBox("Edit line!");
        } break;
        case ID_EDIT_TRANSFORMER: {
            wxMessageBox("Edit transformer!");
        } break;
        case ID_LINE_ADD_NODE: {
            Line* line = (Line*)element;
            line->AddNode(m_camera->GetMousePosition());
            Redraw();
        } break;
        case ID_LINE_REMOVE_NODE: {
            Line* line = (Line*)element;
            line->RemoveNode(m_camera->GetMousePosition());
            Redraw();
        } break;
        case ID_ROTATE_CLOCK: {
            element->Rotate();
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* iElement = *it;
                // Parent's element rotating...
                for(int i = 0; i < (int)iElement->GetParentList().size(); i++) {
                    Element* parent = iElement->GetParentList()[i];
                    if(parent == element) {
                        iElement->RotateNode(parent);
                    }
                }
            }
            Redraw();
        } break;
        case ID_ROTATE_COUNTERCLOCK: {
            element->Rotate(false);
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* iElement = *it;
                // Parent's element rotating...
                for(int i = 0; i < (int)iElement->GetParentList().size(); i++) {
                    Element* parent = iElement->GetParentList()[i];
                    if(parent == element) {
                        iElement->RotateNode(parent, false);
                    }
                }
            }
            Redraw();
        } break;
        case ID_DELETE: {
            for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
                Element* iElement = *it;

                if(element == iElement) {
                    for(auto itp = m_elementList.begin(); itp != m_elementList.end(); ++itp) {
                        Element* child = *itp;
                        // Parent's element being deleted...
                        for(int i = 0; i < (int)child->GetParentList().size(); i++) {
                            Element* parent = child->GetParentList()[i];
                            if(parent == element) {
                                child->RemoveParent(parent);
                            }
                        }
                    }
                    m_elementList.erase(it--);
                }
            }
        } break;
    }
}

void Workspace::RotateSelectedElements(bool clockwise)
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;
        // Parent's element rotating...
        for(int i = 0; i < (int)element->GetParentList().size(); i++) {
            Element* parent = element->GetParentList()[i];
            if(parent) {  // Check if parent is not null
                if(parent->IsSelected()) {
                    element->RotateNode(parent, clockwise);
                    // Update the positions used on motion action, the element will not be necessarily
                    // moved.
                    element->StartMove(m_camera->GetMousePosition());
                }
            }
        }
        if(element->IsSelected()) {
            element->Rotate(clockwise);
            element->StartMove(m_camera->GetMousePosition());
        }
    }
    
    //Rotate text element
    for(auto it = m_textList.begin(); it != m_textList.end(); it++) {
        Text* text = *it;
        if(text->IsSelected()) {
            text->Rotate(clockwise);
            text->StartMove(m_camera->GetMousePosition());
        }
    }
    Redraw();
}

void Workspace::DeleteSelectedElements()
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;

        if(element->IsSelected()) {
            for(auto itp = m_elementList.begin(); itp != m_elementList.end(); ++itp) {
                Element* child = *itp;
                // Parent's element being deleted...
                for(int i = 0; i < (int)child->GetParentList().size(); i++) {
                    Element* parent = child->GetParentList()[i];
                    if(parent == element) {
                        child->RemoveParent(parent);
                    }
                }
            }
            m_elementList.erase(it--);
        }
    }
    Redraw();
}

void Workspace::Fit()
{
    if(m_elementList.size() > 0) {
        wxPoint2DDouble leftUpCorner(0, 0);
        wxPoint2DDouble rightDownCorner(0, 0);
        m_elementList[0]->CalculateBoundaries(leftUpCorner, rightDownCorner);

        for(auto it = m_elementList.begin() + 1; it != m_elementList.end(); it++) {
            Element* element = *it;
            wxPoint2DDouble leftUp;
            wxPoint2DDouble rightDown;
            element->CalculateBoundaries(leftUp, rightDown);
            if(leftUp.m_x < leftUpCorner.m_x) leftUpCorner.m_x = leftUp.m_x;
            if(leftUp.m_y < leftUpCorner.m_y) leftUpCorner.m_y = leftUp.m_y;
            if(rightDown.m_x > rightDownCorner.m_x) rightDownCorner.m_x = rightDown.m_x;
            if(rightDown.m_y > rightDownCorner.m_y) rightDownCorner.m_y = rightDown.m_y;
        }

        int width = 0.0;
        int height = 0.0;
        GetSize(&width, &height);

        double scaleX = double(width) / (rightDownCorner.m_x - leftUpCorner.m_x);
        double scaleY = double(height) / (rightDownCorner.m_y - leftUpCorner.m_y);

        double scale = scaleX < scaleY ? scaleX : scaleY;
        if(scale > m_camera->GetZoomMax()) scale = m_camera->GetZoomMax();
        if(scale < m_camera->GetZoomMin()) scale = m_camera->GetZoomMin();

        m_camera->SetScale(scale);

        m_camera->StartTranslation(leftUpCorner);
        m_camera->SetTranslation(wxPoint2DDouble(0, 0));
        Redraw();
    }
}

void Workspace::ValidateBusesVoltages(Element* initialBus)
{
    double nominalVoltage = ((Bus*)initialBus)->GetEletricalData().nominalVoltage;
    ElectricalUnit nominalVoltageUnit = ((Bus*)initialBus)->GetEletricalData().nominalVoltageUnit;

    for(auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
        Element* child = *it;

        if(typeid(*child) == typeid(Line)) {
            if(child->GetParentList()[0] && child->GetParentList()[1]) {
                BusElectricalData data1 = ((Bus*)child->GetParentList()[0])->GetEletricalData();
                BusElectricalData data2 = ((Bus*)child->GetParentList()[1])->GetEletricalData();

                if(data1.nominalVoltage != data2.nominalVoltage ||
                   data1.nominalVoltageUnit != data2.nominalVoltageUnit) {
                    data1.nominalVoltage = nominalVoltage;
                    data2.nominalVoltage = nominalVoltage;
                    data1.nominalVoltageUnit = nominalVoltageUnit;
                    data2.nominalVoltageUnit = nominalVoltageUnit;

                    ((Bus*)child->GetParentList()[0])->SetElectricalData(data1);
                    ((Bus*)child->GetParentList()[1])->SetElectricalData(data2);

                    it = m_elementList.begin();  // Restart search.
                }
            }
        }
    }

    // ValidateElementsVoltages();
}

void Workspace::ValidateElementsVoltages()
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
        Element* child = *it;

        std::vector<double> nominalVoltage;
        std::vector<ElectricalUnit> nominalVoltageUnit;
        for(int i = 0; i < (int)child->GetParentList().size(); i++) {
            Bus* parent = (Bus*)child->GetParentList()[i];
            if(parent) {
                nominalVoltage.push_back(parent->GetEletricalData().nominalVoltage);
                nominalVoltageUnit.push_back(parent->GetEletricalData().nominalVoltageUnit);
            }
        }
        child->SetNominalVoltage(nominalVoltage, nominalVoltageUnit);
    }
}

bool Workspace::RunPowerFlow()
{
    PowerFlow pf(m_elementList);
    bool result = pf.RunGaussSeidel();
    if(!result) {
        wxMessageDialog msgDialog(this, pf.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }

    Redraw();

    return result;
}
