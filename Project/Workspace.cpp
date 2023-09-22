/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Camera.h"
#include "Element.h"
#include "Workspace.h"
 //#include "Bus.h"
#include "Capacitor.h"
#include "ElementDataObject.h"
#include "HarmCurrent.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

#include "Text.h"

#include "Electromechanical.h"
#include "Fault.h"
#include "PowerFlow.h"
#include "PowerQuality.h"

#include "ChartView.h"
#include "ElementPlotData.h"

#include "PropertiesData.h"

#include "FrequencyResponseForm.h"

#include "HMPlane.h"
#include "Shader.h"
#include "Renderer.h"

// Workspace
Workspace::Workspace() : WorkspaceBase(nullptr) {}
Workspace::Workspace(wxWindow* parent, wxString name, wxStatusBar* statusBar, wxGLContext* sharedGLContext)
    : WorkspaceBase(parent)
{
    m_timer->Start();
    m_name = name;
    m_statusBar = statusBar;
    m_glContext = new wxGLContext(m_glCanvas, sharedGLContext);
    m_camera = new Camera();
    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

    for (int i = 0; i < NUM_ELEMENTS; ++i) { m_elementNumber[i] = 1; }

    const int widths[4] = { -3, -1, 100, 100 };
    m_statusBar->SetStatusWidths(4, widths);

    m_properties = new PropertiesData();

    m_glCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    m_width = static_cast<float>(m_glCanvas->GetSize().x) - 1.0;
    m_height = static_cast<float>(m_glCanvas->GetSize().y) - 1.0;
    m_renderer = new Renderer();
}

Workspace::~Workspace()
{
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        if (*it) delete* it;
    }
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) {
        if (*it) delete* it;
    }

    delete m_hmPlane;

    if (m_camera) delete m_camera;
    //if (m_isThisContextShared) {
    delete m_glContext;
    m_glContext = nullptr;
    //}
    if (m_tipWindow) delete m_tipWindow;
    if (m_properties) delete m_properties;
}

void Workspace::OnPaint(wxPaintEvent& event)
{
    if (!m_glCanvas->IsShown()) return;


    //dc.Clear();

    //m_glContext->SetCurrent(*m_glCanvas);
    m_glCanvas->SetCurrent(*m_glContext);
    wxPaintDC dc(m_glCanvas);

    SetViewport();

    // Set GLCanvas scale and translation
    // Legacy OpenGL
    glScaled(m_camera->GetScale(), m_camera->GetScale(), 0.0);                      // Scale
    glTranslated(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y, 0.0);  // Translation
    // Modern OpenGL
    m_projMatrix = glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);
    //m_projMatrix = glm::scale(m_projMatrix, glm::vec3(m_camera->GetScale(), m_camera->GetScale(), 1.0));
    //m_projMatrix = glm::translate(m_projMatrix, glm::vec3(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y, 0.0));


    m_renderer->Clear();

    // Draw

    // HMPlane
    if (m_hmPlane && m_showHM) {
        m_hmPlane->Draw(*m_renderer, m_projMatrix);
    }

    // Elements
    for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;
        element->Draw(m_camera->GetTranslation(), m_camera->GetScale());
    }

    // Texts
    for (auto it = m_textList.begin(); it != m_textList.end(); ++it) {
        Text* text = *it;
        text->Draw(m_camera->GetTranslation(), m_camera->GetScale());
    }


    if (m_hmPlane && m_showHM) {
        // Reset legacy OpenGL zoom/translation
        glPushMatrix();
        glLoadIdentity();
        //m_projMatrix = glm::mat4(1.0);
        //m_projMatrix = glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f);
        m_hmPlane->DrawLabel(*m_renderer, m_projMatrix, 20.0f, 20.0f);
        glPopMatrix();
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

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    double width = static_cast<double>(m_glCanvas->GetSize().x) - 1.0;
    double height = static_cast<double>(m_glCanvas->GetSize().y) - 1.0;

    // Viewport fit the screen.
    glViewport(0, 0, width, height);
    // Set viewport to legacy OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, height, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Workspace::OnLeftClickDown(wxMouseEvent& event)
{
    wxPoint clickPoint = event.GetPosition();
    bool foundElement = false;
    Element* newElement = nullptr;
    bool showNewElementForm = false;
    bool clickOnSwitch = false;

    if (m_mode == WorkspaceMode::MODE_INSERT_TEXT || m_mode == WorkspaceMode::MODE_PASTE || m_mode == WorkspaceMode::MODE_DRAG_PASTE) {
        m_mode = WorkspaceMode::MODE_EDIT;
    }
    else if (m_mode == WorkspaceMode::MODE_INSERT || m_mode == WorkspaceMode::MODE_DRAG_INSERT || m_mode == WorkspaceMode::MODE_DRAG_INSERT_TEXT) {
        // Get the last element inserted on the list.
        newElement = *(m_elementList.end() - 1);
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            // Clicked in any element.
            if (element->Contains(m_camera->ScreenToWorld(clickPoint))) {
                // Click at a bus.
                if (typeid(*element) == typeid(Bus)) {
                    // Select the bus.
                    element->SetSelected();
                    foundElement = true;  // Element found.
                    // Add the new element's parent. If the element being inserted returns true, back to
                    // edit mode.
                    if (newElement->AddParent(element, m_camera->ScreenToWorld(clickPoint))) {
                        ValidateElementsVoltages();
                        m_timer->Stop();
                        showNewElementForm = true;
                        m_mode = WorkspaceMode::MODE_EDIT;
                    }
                }
            }
        }
        // The line element can have an undefined number of points.
        if (!foundElement) {
            if (typeid(*newElement) == typeid(Line)) { newElement->AddPoint(m_camera->ScreenToWorld(clickPoint)); }
        }
        foundElement = true;
    }
    else {
        bool clickPickbox = false;
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            PowerElement* element = *it;
            element->ResetPickboxes();  // Reset pickbox state.

            // Set movement initial position (not necessarily will be moved).
            element->StartMove(m_camera->ScreenToWorld(clickPoint));

            // Click in selected element node.
            if (element->NodeContains(m_camera->ScreenToWorld(clickPoint)) != 0 && element->IsSelected()) {
                m_mode = WorkspaceMode::MODE_MOVE_NODE;
                m_disconnectedElement = true;
                foundElement = true;
            }

            // Click in an element.
            else if (element->Contains(m_camera->ScreenToWorld(clickPoint))) {
                if (!foundElement) {
                    // Select and show pickbox.
                    element->SetSelected();
                    element->ShowPickbox();
                    foundElement = true;
                }
                // If pickbox contains the click, move the pickbox
                if (element->PickboxContains(m_camera->ScreenToWorld(clickPoint))) {
                    m_mode = WorkspaceMode::MODE_MOVE_PICKBOX;
                    clickPickbox = true;
                }
                // If didn't found a pickbox, move the element
                if (!clickPickbox) { m_mode = WorkspaceMode::MODE_MOVE_ELEMENT; }
            }

            // Click in a switch.
            else if (element->SwitchesContains(m_camera->ScreenToWorld(clickPoint))) {
                element->SetOnline(element->IsOnline() ? false : true);
                clickOnSwitch = true;
            }
        }

        // Text element
        for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) {
            Text* text = *it;

            text->StartMove(m_camera->ScreenToWorld(clickPoint));

            if (text->Contains(m_camera->ScreenToWorld(clickPoint))) {
                if (!foundElement) {
                    text->SetSelected();
                    m_mode = WorkspaceMode::MODE_MOVE_ELEMENT;
                    foundElement = true;
                }
            }
        }
    }

    if (!foundElement) {
        m_mode = WorkspaceMode::MODE_SELECTION_RECT;
        m_startSelRect = m_camera->ScreenToWorld(clickPoint);
    }

    Redraw();
    UpdateStatusBar();

    if (showNewElementForm) {
        if (newElement) {
            newElement->ShowForm(this, newElement);
            if (m_continuousCalc) RunStaticStudies();
        }
    }
    if (clickOnSwitch && m_continuousCalc) RunStaticStudies();

    event.Skip();
}

void Workspace::OnLeftDoubleClick(wxMouseEvent& event)
{
    bool elementEdited = false;
    bool clickOnSwitch = false;
    bool redraw = false;

    for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        PowerElement* element = *it;

        // Click in an element.
        if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
            bool elementIsBus = false;
            Bus oldBus;
            Bus* currentBus = nullptr;
            if ((currentBus = dynamic_cast<Bus*>(element))) {
                elementIsBus = true;
                oldBus = *currentBus;
            }
            m_timer->Stop();
            element->ShowForm(this, element);
            elementEdited = true;
            redraw = true;

            // If the edited element is a bus and was changed the rated voltage, this voltage must be
            // propagated through the lines
            if (elementIsBus) {
                // The voltage was changed
                if (oldBus.GetElectricalData().nominalVoltage != currentBus->GetElectricalData().nominalVoltage ||
                    oldBus.GetElectricalData().nominalVoltageUnit !=
                    currentBus->GetElectricalData().nominalVoltageUnit) {
                    // Check if the bus has line as child.
                    std::vector<Element*> childList = element->GetChildList();
                    for (auto itc = childList.begin(), itcEnd = childList.end(); itc != itcEnd; ++itc) {
                        Element* child = *itc;
                        if (typeid(*child) == typeid(Line)) {
                            wxMessageDialog msgDialog(this, _("Do you want to change the rated voltage of the path?"),
                                _("Warning"), wxYES_NO | wxCENTRE | wxICON_WARNING);
                            if (msgDialog.ShowModal() == wxID_YES)
                                ValidateBusesVoltages(element);
                            else {
                                auto data = currentBus->GetElectricalData();
                                data.nominalVoltage = oldBus.GetElectricalData().nominalVoltage;
                                data.nominalVoltageUnit = oldBus.GetElectricalData().nominalVoltageUnit;
                                currentBus->SetElectricalData(data);
                            }
                            break;
                        }
                    }
                }
                ValidateElementsVoltages();
            }
        }

        // Click in a switch.
        else if (element->SwitchesContains(m_camera->ScreenToWorld(event.GetPosition()))) {
            element->SetOnline(element->IsOnline() ? false : true);
            clickOnSwitch = true;
        }
    }

    // Text element
    for (auto it = m_textList.begin(); it != m_textList.end(); ++it) {
        Text* text = *it;
        if (text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
            text->ShowForm(this, GetElementList());
            redraw = true;
        }
    }
    if (elementEdited) {
        UpdateTextElements();
        if (m_continuousCalc) RunStaticStudies();
    }
    if (clickOnSwitch && m_continuousCalc) RunStaticStudies();

    if (redraw) Redraw();
    m_timer->Start();
}

void Workspace::OnRightClickDown(wxMouseEvent& event)
{
    bool redraw = false;
    if (m_mode == WorkspaceMode::MODE_EDIT) {
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if (element->IsSelected()) {
                // Show context menu.
                if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                    element->ShowPickbox(false);
                    wxMenu menu;
                    if (element->GetContextMenu(menu)) {
                        m_timer->Stop();
                        menu.SetClientData(element);
                        menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &Workspace::OnPopupClick, this);
                        PopupMenu(&menu);
                        redraw = true;

                        if (!menu.GetClientData()) break;
                    }
                    element->ResetPickboxes();
                }
            }
        }
    }
    if (redraw) Redraw();
    m_timer->Start();
}

void Workspace::OnLeftClickUp(wxMouseEvent& event)
{
    // This event (under certain conditions) deselects the elements and back to edit mode or select the elements using
    // the selection rectangle.
    bool foundPickbox = false;
    bool findNewParent = false;
    bool updateVoltages = false;
    auto itnp = m_elementList.begin();

    for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;

        // The user was moving a pickbox.
        if (m_mode == WorkspaceMode::MODE_MOVE_PICKBOX) {
            // Catch only the element that have the pickbox shown.
            if (element->IsPickboxShown()) {
                // If the element is a bus, check if a node is outside.
                if (typeid(*element) == typeid(Bus)) {
                    // Get all the bus children.
                    for (int i = 0; i < (int)m_elementList.size(); i++) {
                        Element* child = m_elementList[i];
                        for (int j = 0; j < (int)child->GetParentList().size(); j++) {
                            Element* parent = child->GetParentList()[j];
                            // The child have a parent that is the element.
                            if (parent == element) {
                                child->UpdateNodes();
                                m_disconnectedElement = true;
                            }
                        }
                    }
                }
            }
        }

        if (m_mode == WorkspaceMode::MODE_SELECTION_RECT) {
            if (element->Intersects(m_selectionRect)) {
                element->SetSelected();
            }
            else if (!event.ControlDown()) {
                element->SetSelected(false);
            }
        }
        else if (m_mode == WorkspaceMode::MODE_MOVE_NODE) {
            if (element->IsSelected()) {
                for (int i = 0; i < (int)m_elementList.size(); i++) {
                    Element* parent = m_elementList[i];
                    if (typeid(*parent) == typeid(Bus)) {
                        if (element->SetNodeParent(parent)) {
                            parent->AddChild(element);
                            findNewParent = true;
                            itnp = it;
                            element->ResetNodes();
                            break;
                        }
                    }
                }
                // element->ResetNodes();
            }
        }
        else {
            // Deselect
            if (!event.ControlDown()) {
                if (!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { element->SetSelected(false); }
            }

            if (element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
                foundPickbox = true;
            }
            else {
                element->ShowPickbox(false);
                element->ResetPickboxes();
            }
        }
    }

    // Text element
    for (auto it = m_textList.begin(); it != m_textList.end(); it++) {
        Text* text = *it;
        if (m_mode == WorkspaceMode::MODE_SELECTION_RECT) {
            if (text->Intersects(m_selectionRect)) {
                text->SetSelected();
            }
            else if (!event.ControlDown()) {
                text->SetSelected(false);
            }
        }
        else if (!event.ControlDown()) {
            if (!text->Contains(m_camera->ScreenToWorld(event.GetPosition()))) { text->SetSelected(false); }
        }
    }

    if (findNewParent) {
        std::rotate(itnp, itnp + 1, m_elementList.end());
        updateVoltages = true;
    }
    if (!foundPickbox) { SetCursor(wxCURSOR_ARROW); }

    if (m_mode != WorkspaceMode::MODE_INSERT) { m_mode = WorkspaceMode::MODE_EDIT; }

    if (updateVoltages) { ValidateElementsVoltages(); }

    if (m_continuousCalc && m_disconnectedElement) {
        m_disconnectedElement = false;
        RunStaticStudies();
    }

    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

    if (m_hmPlane && m_showHM) {
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }

    Redraw();
    UpdateStatusBar();
}

void Workspace::OnMouseMotion(wxMouseEvent& event)
{
    bool redraw = false;
    switch (m_mode) {
    case WorkspaceMode::MODE_INSERT: {
        Element* newElement = *(m_elementList.end() - 1);  // Get the last element in the list.
        newElement->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
        redraw = true;
    } break;

    case WorkspaceMode::MODE_INSERT_TEXT: {
        Text* newText = *(m_textList.end() - 1);
        newText->SetPosition(m_camera->ScreenToWorld(event.GetPosition()));
        redraw = true;
    } break;

    case WorkspaceMode::MODE_DRAG:
    case WorkspaceMode::MODE_DRAG_INSERT:
    case WorkspaceMode::MODE_DRAG_INSERT_TEXT:
    case WorkspaceMode::MODE_DRAG_PASTE: {
        m_camera->SetTranslation(event.GetPosition());
        redraw = true;
    } break;

    case WorkspaceMode::MODE_EDIT: {
        bool foundPickbox = false;
        for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* element = *it;
            if (element->IsSelected()) {
                // Show element pickbox (when it has) if the mouse is over the selected object.
                if (element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                    element->ShowPickbox();
                    redraw = true;

                    // If the mouse is over a pickbox set correct mouse cursor.
                    if (element->PickboxContains(m_camera->ScreenToWorld(event.GetPosition()))) {
                        foundPickbox = true;
                        SetCursor(element->GetBestPickboxCursor());
                    }
                    else if (!foundPickbox) {
                        SetCursor(wxCURSOR_ARROW);
                        element->ResetPickboxes();
                    }
                }
                else if (!foundPickbox) {
                    if (element->IsPickboxShown()) redraw = true;

                    element->ShowPickbox(false);
                    element->ResetPickboxes();
                    SetCursor(wxCURSOR_ARROW);
                }
            }
        }
    } break;

    case WorkspaceMode::MODE_MOVE_NODE: {
        for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* element = *it;
            if (element->IsSelected()) {
                element->MoveNode(nullptr, m_camera->ScreenToWorld(event.GetPosition()));
                redraw = true;
            }
        }
    } break;

    case WorkspaceMode::MODE_MOVE_PICKBOX: {
        for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* element = *it;
            if (element->IsSelected()) {
                element->MovePickbox(m_camera->ScreenToWorld(event.GetPosition()));
                redraw = true;
            }
        }
        if (m_hmPlane && m_showHM) {
            m_hmPlane->Clear();
        }
    } break;

    case WorkspaceMode::MODE_MOVE_ELEMENT:
    case WorkspaceMode::MODE_PASTE: {
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if (element->IsSelected()) {
                element->Move(m_camera->ScreenToWorld(event.GetPosition()));
                // Move child nodes
                std::vector<Element*> childList = element->GetChildList();
                for (auto it = childList.begin(), itEnd = childList.end(); it != itEnd; ++it) {
                    (*it)->MoveNode(element, m_camera->ScreenToWorld(event.GetPosition()));
                }
                redraw = true;
            }
        }
        // Text element motion
        for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; it++) {
            Text* text = *it;
            if (text->IsSelected()) {
                text->Move(m_camera->ScreenToWorld(event.GetPosition()));
                redraw = true;
            }
        }
        if (m_hmPlane && m_showHM) {
            m_hmPlane->Clear();
        }
    } break;

    case WorkspaceMode::MODE_SELECTION_RECT: {
        wxPoint2DDouble currentPos = m_camera->ScreenToWorld(event.GetPosition());
        double x, y, w, h;
        if (currentPos.m_x < m_startSelRect.m_x) {
            x = currentPos.m_x;
            w = m_startSelRect.m_x - currentPos.m_x;
        }
        else {
            x = m_startSelRect.m_x;
            w = currentPos.m_x - m_startSelRect.m_x;
        }
        if (currentPos.m_y < m_startSelRect.m_y) {
            y = currentPos.m_y;
            h = m_startSelRect.m_y - currentPos.m_y;
        }
        else {
            y = m_startSelRect.m_y;
            h = currentPos.m_y - m_startSelRect.m_y;
        }

        m_selectionRect = wxRect2DDouble(x, y, w, h);
        redraw = true;
    } break;
    }

    if (redraw) {
        Redraw();
    }
    m_camera->UpdateMousePosition(event.GetPosition());
    UpdateStatusBar();
    m_timer->Start();  // Restart the timer.
    event.Skip();
}

void Workspace::OnMiddleDown(wxMouseEvent& event)
{
    // Set to drag mode.
    switch (m_mode) {
    case WorkspaceMode::MODE_INSERT: {
        m_mode = WorkspaceMode::MODE_DRAG_INSERT;
    } break;
    case WorkspaceMode::MODE_INSERT_TEXT: {
        m_mode = WorkspaceMode::MODE_DRAG_INSERT_TEXT;
    } break;
    case WorkspaceMode::MODE_PASTE: {
        m_mode = WorkspaceMode::MODE_DRAG_PASTE;
    } break;
    default: {
        m_mode = WorkspaceMode::MODE_DRAG;
    } break;
    }
    m_camera->StartTranslation(m_camera->ScreenToWorld(event.GetPosition()));
    UpdateStatusBar();

    if (m_hmPlane && m_showHM) {
        m_hmPlane->Clear();
    }

    event.Skip();
}

void Workspace::OnMiddleUp(wxMouseEvent& event)
{
    switch (m_mode) {
    case WorkspaceMode::MODE_DRAG_INSERT: {
        m_mode = WorkspaceMode::MODE_INSERT;
    } break;
    case WorkspaceMode::MODE_DRAG_INSERT_TEXT: {
        m_mode = WorkspaceMode::MODE_INSERT_TEXT;
    } break;
    case WorkspaceMode::MODE_DRAG_PASTE: {
        m_mode = WorkspaceMode::MODE_PASTE;
    } break;
    case WorkspaceMode::MODE_INSERT:
    case WorkspaceMode::MODE_INSERT_TEXT:
    case WorkspaceMode::MODE_PASTE: {
        // Does nothing.
    } break;
    default: {
        m_mode = WorkspaceMode::MODE_EDIT;
    } break;
    }
    UpdateStatusBar();

    if (m_hmPlane && m_showHM) {
        UpdateHeatMap();
        Redraw();
    }

    event.Skip();
}

void Workspace::OnScroll(wxMouseEvent& event)
{
    if (event.GetWheelRotation() > 0)
        m_camera->SetScale(event.GetPosition(), +0.05);
    else
        m_camera->SetScale(event.GetPosition(), -0.05);

    if (m_hmPlane && m_showHM) {
        m_hmPlane->Clear();
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }

    UpdateStatusBar();
    Redraw();
}

void Workspace::OnKeyDown(wxKeyEvent& event)
{
    bool insertingElement = false;
    if (m_mode == WorkspaceMode::MODE_INSERT || m_mode == WorkspaceMode::MODE_INSERT_TEXT) insertingElement = true;

    char key = event.GetUnicodeKey();
    if (key != WXK_NONE) {
        switch (key) {
        case WXK_ESCAPE:  // Cancel operations.
        {
            if (m_mode == WorkspaceMode::MODE_INSERT) {
                m_elementList.pop_back();  // Removes the last element being inserted.
                m_mode = WorkspaceMode::MODE_EDIT;
                Redraw();
            }
            else if (m_mode == WorkspaceMode::MODE_INSERT_TEXT) {
                m_textList.pop_back();
                m_mode = WorkspaceMode::MODE_EDIT;
                Redraw();
            }
        } break;
        case WXK_DELETE:  // Delete selected elements
        {
            DeleteSelectedElements();
        } break;
        case 'A': {
            if (!insertingElement) {
                Text* newBus = new Text(m_camera->ScreenToWorld(event.GetPosition()));
                m_textList.push_back(newBus);
                m_mode = WorkspaceMode::MODE_INSERT_TEXT;
                m_statusBar->SetStatusText(_("Insert Text: Click to insert, ESC to cancel."));
                Redraw();
            }
        } break;
        case 'F': {
            if (event.GetModifiers() == wxMOD_SHIFT) { Fit(); }
        } break;
        case 'R':  // Rotate the selected elements.
        {
            RotateSelectedElements(event.GetModifiers() != wxMOD_SHIFT);
        } break;
        case 'B':  // Insert a bus.
        {
            if (!insertingElement) {
                Bus* newBus = new Bus(m_camera->ScreenToWorld(event.GetPosition()),
                    wxString::Format(_("Bus %d"), GetElementNumber(ID_BUS)));
                IncrementElementNumber(ID_BUS);
                m_elementList.push_back(newBus);
                m_mode = WorkspaceMode::MODE_INSERT;
                m_statusBar->SetStatusText(_("Insert Bus: Click to insert, ESC to cancel."));
                Redraw();
            }
        } break;
        case 'L': {
            if (!insertingElement) {
                if (!event.ControlDown() && event.ShiftDown()) {  // Insert a load.
                    Load* newLoad = new Load(wxString::Format(_("Load %d"), GetElementNumber(ID_LOAD)));
                    IncrementElementNumber(ID_LOAD);
                    m_elementList.push_back(newLoad);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Load: Click on a buses, ESC to cancel."));
                }
                else if (!event.ControlDown() && !event.ShiftDown()) {  // Insert a power line.
                    Line* newLine = new Line(wxString::Format(_("Line %d"), GetElementNumber(ID_LINE)));
                    IncrementElementNumber(ID_LINE);
                    m_elementList.push_back(newLine);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Line: Click on two buses, ESC to cancel."));
                }
                Redraw();
            }
            // Tests - Ctrl + Shift + L
        } break;
        case 'T':  // Insert a transformer.
        {
            if (!insertingElement) {
                Transformer* newTransformer =
                    new Transformer(wxString::Format(_("Transformer %d"), GetElementNumber(ID_TRANSFORMER)));
                IncrementElementNumber(ID_TRANSFORMER);
                m_elementList.push_back(newTransformer);
                m_mode = WorkspaceMode::MODE_INSERT;
                m_statusBar->SetStatusText(_("Insert Transformer: Click on two buses, ESC to cancel."));
                Redraw();
            }
        } break;
        case 'G':  // Insert a generator.
        {
            if (!insertingElement) {
                SyncGenerator* newGenerator =
                    new SyncGenerator(wxString::Format(_("Generator %d"), GetElementNumber(ID_SYNCGENERATOR)));
                IncrementElementNumber(ID_SYNCGENERATOR);
                m_elementList.push_back(newGenerator);
                m_mode = WorkspaceMode::MODE_INSERT;
                m_statusBar->SetStatusText(_("Insert Generator: Click on a buses, ESC to cancel."));
                Redraw();
            }
        } break;
        case 'I': {
            if (!insertingElement) {
                if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert an inductor.
                    Inductor* newInductor =
                        new Inductor(wxString::Format(_("Inductor %d"), GetElementNumber(ID_INDUCTOR)));
                    IncrementElementNumber(ID_INDUCTOR);
                    m_elementList.push_back(newInductor);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Inductor: Click on a buses, ESC to cancel."));
                }
                else  // Insert an induction motor.
                {
                    IndMotor* newIndMotor =
                        new IndMotor(wxString::Format(_("Induction motor %d"), GetElementNumber(ID_INDMOTOR)));
                    IncrementElementNumber(ID_INDMOTOR);
                    m_elementList.push_back(newIndMotor);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Induction Motor: Click on a buses, ESC to cancel."));
                }
                Redraw();
            }
        } break;
        case 'K':  // Insert a synchronous condenser.
        {
            if (!insertingElement) {
                SyncMotor* newSyncCondenser =
                    new SyncMotor(wxString::Format(_("Synchronous condenser %d"), GetElementNumber(ID_SYNCMOTOR)));
                IncrementElementNumber(ID_SYNCMOTOR);
                m_elementList.push_back(newSyncCondenser);
                m_mode = WorkspaceMode::MODE_INSERT;
                m_statusBar->SetStatusText(_("Insert Synchronous Condenser: Click on a buses, ESC to cancel."));
                Redraw();
            }
        } break;
        case 'C': {
            if (!insertingElement) {
                if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert a capacitor.
                    Capacitor* newCapacitor =
                        new Capacitor(wxString::Format(_("Capacitor %d"), GetElementNumber(ID_CAPACITOR)));
                    IncrementElementNumber(ID_CAPACITOR);
                    m_elementList.push_back(newCapacitor);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(_("Insert Capacitor: Click on a buses, ESC to cancel."));
                    Redraw();
                }
                else if (event.GetModifiers() == wxMOD_CONTROL) {  // Copy.
                    CopySelection();
                }
            }
        } break;
        case 'H': {
            if (!insertingElement) {
                if (event.ShiftDown() && event.ControlDown()) {
                    if (!m_showHM) {
                        m_showHM = true;
                        UpdateHeatMap();
                    }
                    else {
                        m_showHM = false;
                    }

                }
                else if (event.GetModifiers() == wxMOD_SHIFT) {  // Insert an harmonic current source.
                    HarmCurrent* newHarmCurrent = new HarmCurrent(
                        wxString::Format(_("Harmonic Current %d"), GetElementNumber(ID_HARMCURRENT)));
                    IncrementElementNumber(ID_HARMCURRENT);
                    m_elementList.push_back(newHarmCurrent);
                    m_mode = WorkspaceMode::MODE_INSERT;
                    m_statusBar->SetStatusText(
                        _("Insert Harmonic Current Source: Click on a buses, ESC to cancel."));
                }
                Redraw();
            }
        } break;
        case 'V': {
            if (!insertingElement) {
                if (event.GetModifiers() == wxMOD_CONTROL) { Paste(); }
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
    switch (m_mode) {
    case WorkspaceMode::MODE_DRAG: {
        m_statusBar->SetStatusText(_("MODE: DRAG"), 1);
    } break;

    case WorkspaceMode::MODE_PASTE:
    case WorkspaceMode::MODE_DRAG_PASTE: {
        m_statusBar->SetStatusText(_("MODE: PASTE"), 1);
    }

    case WorkspaceMode::MODE_INSERT:
    case WorkspaceMode::MODE_INSERT_TEXT:
    case WorkspaceMode::MODE_DRAG_INSERT:
    case WorkspaceMode::MODE_DRAG_INSERT_TEXT: {
        m_statusBar->SetStatusText(_("MODE: INSERT"), 1);
    } break;

    case WorkspaceMode::MODE_MOVE_ELEMENT:
    case WorkspaceMode::MODE_MOVE_PICKBOX:
    case WorkspaceMode::MODE_MOVE_NODE:
    case WorkspaceMode::MODE_SELECTION_RECT:
    case WorkspaceMode::MODE_EDIT: {
        m_statusBar->SetStatusText(wxT(""));
        m_statusBar->SetStatusText(_("MODE: EDIT"), 1);
    } break;
    }

    m_statusBar->SetStatusText(wxString::Format(_("ZOOM: %d%%"), (int)(m_camera->GetScale() * 100.0)), 2);
    m_statusBar->SetStatusText(
        wxString::Format(wxT("X: %.1f  Y: %.1f"), m_camera->GetMousePosition().m_x, m_camera->GetMousePosition().m_y),
        3);
}

void Workspace::UpdateHeatMap()
{
    if (m_hmPlane && m_showHM) {
        m_hmPlane->Clear();

        wxRect2DDouble screenRect(-100, -100, m_width + 200.0, m_height + 200.0);

        // Get new voltage range
        std::vector<Bus*> busList;
        float minVoltage, maxVoltage;
        if (m_hmAutomaticLabel) {
            minVoltage = 10.0f;
            maxVoltage = 0.0f;
        }
        else {
            minVoltage = m_hmPlane->GetMinLimit();
            maxVoltage = m_hmPlane->GetMaxLimit();
        }

        for (auto* element : m_elementList) {
            if (Bus* bus = dynamic_cast<Bus*>(element)) {
                if (m_hmAutomaticLabel) {
                    const float voltage = std::abs(bus->GetElectricalData().voltage);
                    if (minVoltage > voltage) minVoltage = voltage;
                    if (maxVoltage < voltage) maxVoltage = voltage;
                }
                busList.push_back(bus);
            }
        }
        if (m_hmAutomaticLabel) {
            m_hmPlane->SetLabelLimits(minVoltage, maxVoltage);
        }

        for (Bus* bus : busList) {
            const float voltage = std::abs(bus->GetElectricalData().voltage);
            float depth = 2.0f * (voltage - (maxVoltage + minVoltage) / 2.0f) / (maxVoltage - minVoltage);
            if (depth < -1.0) depth = -1.0;
            if (depth > 1.0) depth = 1.0;

            wxRect2DDouble rect = bus->GetRect();
            rect = wxRect2DDouble(
                (rect.m_x - 100.0f) * m_camera->GetScale() + m_camera->GetTranslation().m_x * m_camera->GetScale(),
                (rect.m_y - 50.0f) * m_camera->GetScale() + m_camera->GetTranslation().m_y * m_camera->GetScale(),
                (rect.m_width + 200.0f) * m_camera->GetScale(),
                (rect.m_height + 100.0f) * m_camera->GetScale());

            if (screenRect.Contains(rect))
                m_hmPlane->SetRectSlope(rect, M_PI * static_cast<float>(bus->GetAngle()) / 180.0f, depth);
        }

        m_hmPlane->UpdateCoords();

        int iterations = std::lround(3.3 * std::pow(m_camera->GetScale(), 0.62));
        if (iterations < 1) iterations = 1;
        m_hmPlane->SmoothPlane(iterations);
    }
}

void Workspace::OnPopupClick(wxCommandEvent& event)
{
    bool redrawHM = false;

    wxMenu* menu = static_cast<wxMenu*>(event.GetEventObject());
    Element* element = static_cast<Element*>(menu->GetClientData());
    switch (event.GetId()) {
    case ID_EDIT_ELEMENT: {
        if (element->ShowForm(this, element)) UpdateTextElements();
    } break;
    case ID_LINE_ADD_NODE: {
        Line* line = static_cast<Line*>(element);
        line->AddNode(m_camera->GetMousePosition());
        Redraw();
    } break;
    case ID_LINE_REMOVE_NODE: {
        Line* line = static_cast<Line*>(element);
        line->RemoveNode(m_camera->GetMousePosition());
        Redraw();
    } break;
    case ID_ROTATE_CLOCK: {
        element->Rotate();
        for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* iElement = *it;
            // Parent's element rotating...
            for (int i = 0; i < (int)iElement->GetParentList().size(); i++) {
                Element* parent = iElement->GetParentList()[i];
                if (parent == element) { iElement->RotateNode(parent); }
            }
        }
        redrawHM = true;
        Redraw();
    } break;
    case ID_ROTATE_COUNTERCLOCK: {
        element->Rotate(false);
        for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
            Element* iElement = *it;
            // Parent's element rotating...
            for (int i = 0; i < (int)iElement->GetParentList().size(); i++) {
                Element* parent = iElement->GetParentList()[i];
                if (parent == element) { iElement->RotateNode(parent, false); }
            }
        }
        redrawHM = true;
        Redraw();
    } break;
    case ID_DELETE: {
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* iElement = *it;

            if (element == iElement) {
                // Remove child/parent.
                std::vector<Element*> childList = element->GetChildList();
                for (auto itc = childList.begin(), itEnd = childList.end(); itc != itEnd; ++itc) {
                    Element* child = *itc;
                    if (child) {
                        child->RemoveParent(element);
                        element->RemoveChild(child);
                    }
                }
                std::vector<Element*> parentList = element->GetParentList();
                for (auto itp = parentList.begin(), itEnd = parentList.end(); itp != itEnd; ++itp) {
                    Element* parent = *itp;
                    if (parent) { parent->RemoveChild(element); }
                }

                for (auto itt = m_textList.begin(); itt != m_textList.end(); ++itt) {
                    Text* text = *itt;
                    if (text->GetElement() == element) {
                        m_textList.erase(itt--);
                        if (text) delete text;
                    }
                }

                m_elementList.erase(it);
                if (element) delete element;
                menu->SetClientData(nullptr);
                break;
            }
        }
    } break;
    }
    if (redrawHM && m_hmPlane && m_showHM) {
        m_hmPlane->Clear();
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }
}

void Workspace::RotateSelectedElements(bool clockwise)
{
    for (auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;
        // Parent's element rotating...
        for (int i = 0; i < (int)element->GetParentList().size(); i++) {
            Element* parent = element->GetParentList()[i];
            if (parent) {  // Check if parent is not null
                if (parent->IsSelected()) {
                    element->RotateNode(parent, clockwise);
                    // Update the positions used on motion action, the element will not be necessarily
                    // moved.
                    element->StartMove(m_camera->GetMousePosition());
                }
            }
        }
        if (element->IsSelected()) {
            element->Rotate(clockwise);
            element->StartMove(m_camera->GetMousePosition());
        }
    }

    // Rotate text element
    for (auto it = m_textList.begin(); it != m_textList.end(); it++) {
        Text* text = *it;
        if (text->IsSelected()) {
            text->Rotate(clockwise);
            text->StartMove(m_camera->GetMousePosition());
        }
    }
    if (m_hmPlane && m_showHM) {
        m_hmPlane->Clear();
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }
    Redraw();
}

void Workspace::DeleteSelectedElements()
{
    // Don't set the end of the list at the loop's begin.
    for (auto it = m_elementList.begin(); it != m_elementList.end();) {
        Element* element = *it;

        if (element->IsSelected()) {
            // Remove child/parent.
            std::vector<Element*> childList = element->GetChildList();
            for (auto itc = childList.begin(), itEnd = childList.end(); itc != itEnd; ++itc) {
                Element* child = *itc;
                if (child) {
                    child->RemoveParent(element);
                    element->RemoveChild(child);
                }
            }
            std::vector<Element*> parentList = element->GetParentList();
            for (auto itp = parentList.begin(), itEnd = parentList.end(); itp != itEnd; ++itp) {
                Element* parent = *itp;
                if (parent) { parent->RemoveChild(element); }
            }

            for (auto itt = m_textList.begin(); itt != m_textList.end(); ++itt) {
                Text* text = *itt;
                if (text->GetElement() == element) {
                    m_textList.erase(itt--);
                    if (text) delete text;
                }
            }

            it = m_elementList.erase(it);
            if (element) delete element;
        }
        else it++;
    }

    for (auto it = m_textList.begin(); it != m_textList.end(); ++it) {
        Text* text = *it;
        if (text->IsSelected()) {
            m_textList.erase(it--);
            if (text) delete text;
        }
    }
    if (m_hmPlane && m_showHM) {
        m_hmPlane->Clear();
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }
    Redraw();
}

bool Workspace::GetElementsCorners(wxPoint2DDouble& leftUpCorner,
    wxPoint2DDouble& rightDownCorner,
    std::vector<Element*> elementList)
{
    if (elementList.size() == 0) return false;

    elementList[0]->CalculateBoundaries(leftUpCorner, rightDownCorner);

    for (auto it = elementList.begin() + 1, itEnd = elementList.end(); it != itEnd; it++) {
        Element* element = *it;
        wxPoint2DDouble leftUp;
        wxPoint2DDouble rightDown;
        element->CalculateBoundaries(leftUp, rightDown);
        if (leftUp.m_x < leftUpCorner.m_x) leftUpCorner.m_x = leftUp.m_x;
        if (leftUp.m_y < leftUpCorner.m_y) leftUpCorner.m_y = leftUp.m_y;
        if (rightDown.m_x > rightDownCorner.m_x) rightDownCorner.m_x = rightDown.m_x;
        if (rightDown.m_y > rightDownCorner.m_y) rightDownCorner.m_y = rightDown.m_y;
    }
    return true;
}

void Workspace::Fit()
{
    wxPoint2DDouble leftUpCorner(0, 0);
    wxPoint2DDouble rightDownCorner(0, 0);
    std::vector<Element*> elementList = GetElementList();
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) { elementList.push_back(*it); }

    if (!GetElementsCorners(leftUpCorner, rightDownCorner, elementList)) return;
    wxPoint2DDouble middleCoords = (leftUpCorner + rightDownCorner) / 2.0;

    int width = 0.0;
    int height = 0.0;
    GetSize(&width, &height);

    double scaleX = double(width) / (rightDownCorner.m_x - leftUpCorner.m_x);
    double scaleY = double(height) / (rightDownCorner.m_y - leftUpCorner.m_y);

    double scale = scaleX < scaleY ? scaleX : scaleY;
    if (scale > m_camera->GetZoomMax()) scale = m_camera->GetZoomMax();
    if (scale < m_camera->GetZoomMin()) scale = m_camera->GetZoomMin();

    m_camera->SetScale(scale);

    m_camera->StartTranslation(middleCoords);
    m_camera->SetTranslation(wxPoint2DDouble(width / 2, height / 2));

    if (m_hmPlane && m_showHM) {
        UpdateHeatMap();
    }

    Redraw();
}

void Workspace::ValidateBusesVoltages(Element* initialBus)
{
    double nominalVoltage = static_cast<Bus*>(initialBus)->GetElectricalData().nominalVoltage;
    ElectricalUnit nominalVoltageUnit = static_cast<Bus*>(initialBus)->GetElectricalData().nominalVoltageUnit;

    for (auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
        Element* child = *it;

        if (typeid(*child) == typeid(Line)) {
            if (child->GetParentList()[0] && child->GetParentList()[1]) {
                BusElectricalData data1 = static_cast<Bus*>(child->GetParentList()[0])->GetElectricalData();
                BusElectricalData data2 = static_cast<Bus*>(child->GetParentList()[1])->GetElectricalData();

                if (data1.nominalVoltage != data2.nominalVoltage ||
                    data1.nominalVoltageUnit != data2.nominalVoltageUnit) {
                    data1.nominalVoltage = nominalVoltage;
                    data2.nominalVoltage = nominalVoltage;
                    data1.nominalVoltageUnit = nominalVoltageUnit;
                    data2.nominalVoltageUnit = nominalVoltageUnit;

                    static_cast<Bus*>(child->GetParentList()[0])->SetElectricalData(data1);
                    static_cast<Bus*>(child->GetParentList()[1])->SetElectricalData(data2);

                    it = m_elementList.begin();  // Restart search.
                }
            }
        }
    }

    // ValidateElementsVoltages();
}

void Workspace::ValidateElementsVoltages()
{
    for (auto it = m_elementList.begin(); it != m_elementList.end(); it++) {
        PowerElement* child = *it;

        std::vector<double> nominalVoltage;
        std::vector<ElectricalUnit> nominalVoltageUnit;
        for (int i = 0; i < (int)child->GetParentList().size(); i++) {
            Bus* parent = static_cast<Bus*>(child->GetParentList()[i]);
            if (parent) {
                nominalVoltage.push_back(parent->GetElectricalData().nominalVoltage);
                nominalVoltageUnit.push_back(parent->GetElectricalData().nominalVoltageUnit);
            }
        }
        child->SetNominalVoltage(nominalVoltage, nominalVoltageUnit);
    }
}

bool Workspace::RunPowerFlow()
{
    auto simProp = m_properties->GetSimulationPropertiesData();
    double basePower = simProp.basePower;
    if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
        basePower *= 1e6;
    else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
        basePower *= 1e3;
    PowerFlow pf(GetElementList());
    bool result = false;
    wxStopWatch sw;
    switch (simProp.powerFlowMethod) {
    case GAUSS_SEIDEL: {
        result = pf.RunGaussSeidel(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
            simProp.initAngle, simProp.accFator);
    } break;
    case NEWTON_RAPHSON: {
        result = pf.RunNewtonRaphson(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
            simProp.initAngle, simProp.newtonInertia);
    } break;
    case GAUSS_NEWTON: {
        result =
            pf.RunGaussNewton(basePower, simProp.powerFlowMaxIterations, simProp.powerFlowTolerance,
                simProp.initAngle, simProp.accFator, simProp.gaussTolerance, simProp.newtonInertia);
    } break;
    }
    sw.Pause();
    if (!result) {
        wxMessageDialog msgDialog(this, pf.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
    else {
        m_statusBar->SetStatusText(
            wxString::Format(_("Power flow converge with %d iterations (%ld ms)"), pf.GetIterations(), sw.Time()));
    }

    UpdateTextElements();
    UpdateHeatMap();
    Redraw();

    return result;
}

bool Workspace::UpdateTextElements()
{
    bool isTexturesOK = true;
    double basePower = m_properties->GetSimulationPropertiesData().basePower;
    if (m_properties->GetSimulationPropertiesData().basePowerUnit == ElectricalUnit::UNIT_kVA)
        basePower *= 1e3;
    else if (m_properties->GetSimulationPropertiesData().basePowerUnit == ElectricalUnit::UNIT_MVA)
        basePower *= 1e6;
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) {
        Text* text = *it;
        text->UpdateText(basePower);
        if (!text->IsGLTextOK()) isTexturesOK = false;
    }
    return isTexturesOK;
}

void Workspace::CopySelection()
{
    UpdateElementsID();
    std::vector<Element*> selectedElements;
    // The buses need to be numerated to associate the child's parents to the copies.
    int busNumber = 0;
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        Element* element = *it;
        if (typeid(*element) == typeid(Bus)) {
            Bus* bus = static_cast<Bus*>(element);
            auto data = bus->GetElectricalData();
            data.number = busNumber;
            bus->SetElectricalData(data);
            busNumber++;
        }
        if (element->IsSelected()) { selectedElements.push_back(element); }
    }
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) {
        Text* text = *it;
        if (text->IsSelected()) { selectedElements.push_back(text); }
    }
    ElementDataObject* dataObject = new ElementDataObject(selectedElements);
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(dataObject);
        wxTheClipboard->Close();
    }
}

bool Workspace::Paste()
{
    if (wxTheClipboard->Open()) {
        ElementDataObject dataObject;

        if (wxTheClipboard->IsSupported(dataObject.GetFormat())) {
            if (!wxTheClipboard->GetData(dataObject)) {
                wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
                    wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
                dialog.ShowModal();
                wxTheClipboard->Close();
                return false;
            }
        }
        else {
            wxTheClipboard->Close();
            return false;
        }
        wxTheClipboard->Close();

        UnselectAll();

        std::vector<Element*> pastedElements;
        ElementsLists* elementsLists = dataObject.GetElementsLists();

        // Paste buses (parents).
        auto parentList = elementsLists->parentList;
        std::vector<Bus*> pastedBusList;  // To set new parents;
        for (auto it = parentList.begin(), itEnd = parentList.end(); it != itEnd; ++it) {
            Element* copy = (*it)->GetCopy();
            if (copy) {
                pastedElements.push_back(copy);
                pastedBusList.push_back(static_cast<Bus*>(copy));
                m_elementList.push_back(static_cast<PowerElement*>(copy));
            }
        }

        // Paste other elements.
        auto elementLists = elementsLists->elementList;
        for (auto it = elementLists.begin(), itEnd = elementLists.end(); it != itEnd; ++it) {
            Element* copy = (*it)->GetCopy();
            if (copy) {
                // Check if is text element
                if (Text* text = dynamic_cast<Text*>(copy)) {
                    // Check if element associated with the text exists.
                    bool elementExist = false;
                    for (int i = 0; i < (int)m_elementList.size(); i++) {
                        if (text->GetElement() == m_elementList[i]) {
                            elementExist = true;
                            break;
                        }
                    }
                    if (elementExist) {
                        pastedElements.push_back(copy);
                        m_textList.push_back(text);
                    }
                }
                else {
                    // Change the parent if copied, otherwise remove it.
                    for (unsigned int j = 0; j < copy->GetParentList().size(); j++) {
                        Bus* currentParent = static_cast<Bus*>(copy->GetParentList()[j]);
                        if (currentParent) {
                            int parentID = currentParent->GetID();
                            bool parentCopied = false;
                            for (int k = 0; k < (int)pastedBusList.size(); k++) {
                                Bus* newParent = pastedBusList[k];
                                if (parentID == newParent->GetID()) {
                                    parentCopied = true;
                                    copy->ReplaceParent(currentParent, newParent);
                                    break;
                                }
                            }
                            if (!parentCopied) copy->RemoveParent(currentParent);
                        }
                    }

                    pastedElements.push_back(copy);
                    m_elementList.push_back(static_cast<PowerElement*>(copy));
                }
            }
        }

        // Update buses childs
        for (auto it = pastedBusList.begin(), itEnd = pastedBusList.end(); it != itEnd; ++it) {
            Bus* bus = *it;
            std::vector<Element*> childList = bus->GetChildList();
            for (auto it = childList.begin(), itEnd = childList.end(); it != itEnd; ++it) {
                Element* currentChild = *it;
                int childID = currentChild->GetID();
                bool childCopied = false;
                for (int i = 0; i < (int)pastedElements.size(); i++) {
                    Element* newChild = pastedElements[i];
                    if (childID == newChild->GetID()) {
                        childCopied = true;
                        bus->ReplaceChild(currentChild, newChild);
                        break;
                    }
                }
                if (!childCopied) bus->RemoveChild(currentChild);
            }
        }

        // Move elements (and nodes) to the mouse position.
        // The start position it's the center of the pasted objects.
        wxPoint2DDouble leftUpCorner, rightDownCorner;
        GetElementsCorners(leftUpCorner, rightDownCorner, pastedElements);
        wxPoint2DDouble startPosition = (leftUpCorner + rightDownCorner) / 2.0;
        for (auto it = pastedElements.begin(), itEnd = pastedElements.end(); it != itEnd; ++it) {
            Element* element = *it;
            element->StartMove(startPosition);
            element->Move(m_camera->GetMousePosition());
            for (int i = 0; i < (int)element->GetParentList().size(); i++) {
                Element* parent = element->GetParentList()[i];
                element->MoveNode(parent, m_camera->GetMousePosition());
            }
        }
    }
    else {
        wxMessageDialog dialog(this, _("It was not possible to paste from clipboard."), _("Error"),
            wxOK | wxCENTER | wxICON_ERROR, wxDefaultPosition);
        dialog.ShowModal();
        return false;
    }

    UpdateElementsID();
    m_mode = WorkspaceMode::MODE_PASTE;
    m_statusBar->SetStatusText(_("Click to paste."));
    UpdateStatusBar();
    Redraw();
    return true;
}

void Workspace::UnselectAll()
{
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; it++) {
        Element* element = *it;
        element->SetSelected(false);
    }
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; it++) {
        Text* text = *it;
        text->SetSelected(false);
    }
}

void Workspace::EnableHeatMap(const bool& enable)
{
    m_showHM = enable;
    if (m_showHM) {
        UpdateHeatMap();
    }
    Redraw();
}

void Workspace::UpdateElementsID()
{
    int id = 0;
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        Element* element = *it;
        element->SetID(id);
        id++;
    }
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) {
        Text* text = *it;
        text->SetID(id);
        id++;
    }
}
void Workspace::OnTimer(wxTimerEvent& event)
{
    if (m_tipWindow) {
        m_tipWindow->Close();
        m_tipWindow = nullptr;
    }
    if (m_mode == WorkspaceMode::MODE_EDIT) {
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if (element->Contains(m_camera->GetMousePosition())) {
                wxString tipText = element->GetTipText();
                if (!tipText.IsEmpty()) {
                    m_tipWindow = new wxTipWindow(this, tipText, 10000, &m_tipWindow);
                    // Creates a very tiny bounding rect to remove the tip on any mouse movement.
                    m_tipWindow->SetBoundingRect(wxRect(wxGetMousePosition(), wxSize(1, 1)));
                    break;
                }
            }
        }
    }

    m_timer->Stop();
}

void Workspace::SetTextList(std::vector<Text*> textList)
{
    m_textList.clear();
    for (auto it = textList.begin(), itEnd = textList.end(); it != itEnd; ++it) m_textList.push_back(*it);

    UpdateTextElements();
}

void Workspace::SetElementList(std::vector<Element*> elementList)
{
    m_elementList.clear();
    for (auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it)
        m_elementList.push_back(static_cast<PowerElement*>(*it));
}

void Workspace::OnIdle(wxIdleEvent& event)
{
    // The OpenGL element (m_glCanvas) must be completely initialized (showed) to draw properly the textures and init glew.
    // TODO(?): Find other solution to text displayed wrong on opened file.
    if (m_justOpened) {
        if (UpdateTextElements()) m_justOpened = false;
        if (glewInit() != GLEW_OK)
            wxMessageBox("Erro ao iniciar glew ='(");
        else
        {
            // Load and create shader
            m_basicShader = new Shader("..//data//shaders//Basic.shader");
            m_hmLabelShader = new Shader("..//data//shaders//HMLabel.shader");
            float limits[2] = { 1.05, 0.95 };
            m_hmPlane = new HMPlane(m_basicShader, m_hmLabelShader, m_width, m_height, limits);
        }

        Redraw();
    }
}

std::vector<Element*> Workspace::GetAllElements() const
{
    std::vector<Element*> allElements;

    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) allElements.push_back(*it);
    for (auto it = m_textList.begin(), itEnd = m_textList.end(); it != itEnd; ++it) allElements.push_back(*it);

    return allElements;
}

bool Workspace::RunFault()
{
    Fault fault(GetElementList());
    bool result = fault.RunFaultCalculation(100e6);
    if (!result) {
        wxMessageDialog msgDialog(this, fault.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }

    UpdateTextElements();
    Redraw();

    return result;
}

std::vector<Element*> Workspace::GetElementList() const
{
    std::vector<Element*> elementList;
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) elementList.push_back(*it);
    return elementList;
}

bool Workspace::RunSCPower()
{
    Fault fault(GetElementList());
    bool result = fault.RunSCPowerCalcutation(100e6);
    if (!result) {
        wxMessageDialog msgDialog(this, fault.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }

    UpdateTextElements();
    Redraw();

    return result;
}

bool Workspace::RunStability()
{
    // Run power flow before stability.
    RunPowerFlow();

    Electromechanical stability(this, GetElementList(), m_properties->GetSimulationPropertiesData());
    wxStopWatch sw;
    bool result = stability.RunStabilityCalculation();
    sw.Pause();
    if (!result) {
        wxMessageDialog msgDialog(this, stability.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
    m_stabilityTimeVector.clear();
    m_stabilityTimeVector = stability.GetTimeVector();

    // Run power flow after stability.
    RunPowerFlow();

    wxMessageDialog msgDialog(
        this,
        wxString::Format(_("The program took %ld ms to run this system.\nDo you wish to open the stability graphics?"),
            sw.Time()),
        _("Question"), wxYES_NO | wxCENTRE | wxICON_QUESTION);
    if (msgDialog.ShowModal() == wxID_YES) {
        std::vector<ElementPlotData> plotDataList;
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            PowerElement* element = *it;
            ElementPlotData plotData;
            if (element->GetPlotData(plotData)) plotDataList.push_back(plotData);
        }

        ElementPlotData plotData;
        plotData.SetName(_("Simulation parameters"));
        plotData.SetCurveType(ElementPlotData::CurveType::CT_TEST);
        plotData.AddData(stability.GetIterationVector(), _("Iterations number"));
        plotDataList.push_back(plotData);

        ChartView* cView = new ChartView(this, plotDataList, m_stabilityTimeVector);
        cView->Show();
    }

    return result;
}
void Workspace::OnMiddleDoubleClick(wxMouseEvent& event)
{
    Fit();
    event.Skip();
}

bool Workspace::RunStaticStudies()
{
    bool pfStatus, faultStatus, scStatus, harmStatus;
    pfStatus = faultStatus = scStatus = harmStatus = false;

    pfStatus = RunPowerFlow();

    if (m_properties->GetSimulationPropertiesData().faultAfterPowerFlow) {
        if (pfStatus) faultStatus = RunFault();
    }
    else {
        faultStatus = true;
    }

    if (m_properties->GetSimulationPropertiesData().scPowerAfterPowerFlow) {
        if (pfStatus) scStatus = RunSCPower();
    }
    else {
        scStatus = true;
    }

    if (m_properties->GetSimulationPropertiesData().harmDistortionAfterPowerFlow) {
        if (pfStatus) harmStatus = RunHarmonicDistortion();
    }
    else {
        harmStatus = true;
    }

    if (pfStatus && faultStatus && scStatus && harmStatus) return true;

    return false;
}

bool Workspace::RunHarmonicDistortion()
{
    auto simProp = m_properties->GetSimulationPropertiesData();
    double basePower = simProp.basePower;
    if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
        basePower *= 1e6;
    else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
        basePower *= 1e3;
    if (!RunPowerFlow()) return false;

    PowerQuality pq(GetElementList());
    bool result = pq.CalculateDistortions(basePower);

    if (!result) {
        wxMessageDialog msgDialog(this, pq.GetErrorMessage(), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
    else {
        UpdateTextElements();
        Redraw();
    }    

    return result;
}

bool Workspace::RunFrequencyResponse()
{
    // Get bus list
    std::vector<Bus*> busList;
    for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        if (Bus* bus = dynamic_cast<Bus*>(*it)) { busList.push_back(bus); }
    }

    auto data = m_properties->GetFreqRespData();

    FrequencyResponseForm frForm(this, busList, data.injBusNumber, data.initFreq, data.finalFreq, data.stepFreq);

    if (frForm.ShowModal() == wxID_OK) {
        data.initFreq = frForm.GetInitFreq();
        data.finalFreq = frForm.GetEndFreq();
        data.stepFreq = frForm.GetStepFreq();
        data.injBusNumber = frForm.GetInjBusNumber();
        m_properties->SetFreqRespData(data);
    }
    else
        return false;

    auto simProp = m_properties->GetSimulationPropertiesData();
    double basePower = simProp.basePower;
    if (simProp.basePowerUnit == ElectricalUnit::UNIT_MVA)
        basePower *= 1e6;
    else if (simProp.basePowerUnit == ElectricalUnit::UNIT_kVA)
        basePower *= 1e3;
    PowerQuality pq(GetElementList());
    bool result = pq.CalculateFrequencyResponse(simProp.stabilityFrequency, data.initFreq, data.finalFreq,
        data.stepFreq, data.injBusNumber, basePower);

    wxMessageDialog msgDialog(
        this, wxString::Format(_("Calculations done.\nDo you wish to open the frequency response graphics?")),
        _("Question"), wxYES_NO | wxCENTRE | wxICON_QUESTION);
    if (msgDialog.ShowModal() == wxID_YES) {
        std::vector<ElementPlotData> plotDataList;
        for (auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            PowerElement* element = *it;
            ElementPlotData plotData;
            if (element->GetPlotData(plotData, PlotStudy::FREQRESPONSE)) plotDataList.push_back(plotData);
        }

        ChartView* cView = new ChartView(this, plotDataList, pq.GetFrequencies());
        cView->Show();
    }

    UpdateTextElements();
    Redraw();

    return result;
}
void Workspace::OnResize(wxSizeEvent& event)
{
    m_width = static_cast<float>(m_glCanvas->GetSize().x) - 1.0f;
    m_height = static_cast<float>(m_glCanvas->GetSize().y) - 1.0f;

    if (m_hmPlane && m_showHM) {
        m_hmPlane->Resize(m_width, m_height);
        m_showHMTimer = true;
        m_timerHeatMap->Start();
    }

    event.Skip();
}
void Workspace::OnHeatMapTime(wxTimerEvent& event)
{
    if (m_showHMTimer) {
        UpdateHeatMap();
        Redraw();
        m_showHMTimer = false;
    }
}
