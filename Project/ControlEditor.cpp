#include "ControlEditor.h"

#include "Camera.h"
#include "ControlElement.h"
#include "TransferFunction.h"
#include "ConnectionLine.h"
#include "Sum.h"

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, wxImage image, wxWindowID id)
    : wxWindow(parent, id)
{
    SetBackgroundColour(*wxWHITE);
    // m_font = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    m_label = label;
    m_image = image;
    m_imageSize = wxSize(image.GetWidth(), image.GetHeight());

    // Calculate label size.
    wxScreenDC dc;
    dc.SetFont(m_font);
    wxSize textSize = dc.GetTextExtent(label);

    int buttonWidth = 0;
    if(textSize.GetWidth() > m_imageSize.GetWidth()) {
        buttonWidth = textSize.GetWidth();
        m_imagePosition = wxPoint((buttonWidth - m_imageSize.GetWidth()) / 2 + m_borderSize, m_borderSize);
        m_labelPosition = wxPoint(m_borderSize, m_imageSize.GetHeight() + m_borderSize);
    } else {
        buttonWidth = m_imageSize.GetWidth();
        m_imagePosition = wxPoint(m_borderSize, m_borderSize);
        m_labelPosition =
            wxPoint((buttonWidth - textSize.GetWidth()) / 2 + m_borderSize, m_imageSize.GetHeight() + m_borderSize);
    }
    m_buttonSize =
        wxSize(buttonWidth + 2 * m_borderSize, textSize.GetHeight() + m_imageSize.GetHeight() + 2 * m_borderSize);
    SetMinSize(m_buttonSize + wxSize(m_borderSize, m_borderSize));

    // Events.
    Bind(wxEVT_PAINT, &ControlElementButton::OnPaint, this);
    Bind(wxEVT_ENTER_WINDOW, &ControlElementButton::OnMouseEnter, this);
    Bind(wxEVT_LEAVE_WINDOW, &ControlElementButton::OnMouseLeave, this);
    Bind(wxEVT_LEFT_DOWN, &ControlElementButton::OnLeftClickDown, this);
    Bind(wxEVT_LEFT_UP, &ControlElementButton::OnLeftClickUp, this);
}

ControlElementButton::~ControlElementButton() {}
void ControlElementButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if(gc) {
        if(m_mouseAbove) {
            if(m_selected) {
                gc->SetPen(wxPen(wxColour(0, 125, 255, 255), m_borderSize - 1));
                gc->SetBrush(wxBrush(wxColour(0, 125, 255, 100)));
            } else {
                gc->SetPen(*wxTRANSPARENT_PEN);
                gc->SetBrush(wxBrush(wxColour(0, 125, 255, 70)));
            }
            gc->DrawRectangle(m_borderSize / 2, m_borderSize / 2, m_buttonSize.GetWidth(), m_buttonSize.GetHeight());
        }
        gc->DrawBitmap(gc->CreateBitmapFromImage(m_image), m_imagePosition.x, m_imagePosition.y, m_imageSize.GetWidth(),
            m_imageSize.GetHeight());
        gc->SetFont(m_font, *wxBLACK);
        gc->DrawText(m_label, m_labelPosition.x, m_labelPosition.y);
        delete gc;
    }
}

void ControlElementButton::OnMouseEnter(wxMouseEvent& event)
{
    m_mouseAbove = true;
    Refresh();
    event.Skip();
}

void ControlElementButton::OnMouseLeave(wxMouseEvent& event)
{
    m_mouseAbove = false;
    Refresh();
    event.Skip();
}

void ControlElementButton::OnLeftClickDown(wxMouseEvent& event)
{
    m_selected = true;
    Refresh();
    event.Skip();
}

void ControlElementButton::OnLeftClickUp(wxMouseEvent& event)
{
    m_selected = false;
    Refresh();
    event.Skip();
}

ControlEditor::ControlEditor(wxWindow* parent)
    : ControlEditorBase(parent)
{
    BuildControlElementPanel();
    m_glContext = new wxGLContext(m_glCanvas);
    m_camera = new Camera();
    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);
    // m_camera->SetScale(1.2);
}
ControlEditor::~ControlEditor()
{
    // m_tfButton->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlEditor::LeftClickDown), m_tfButton, this);
}

void ControlEditor::BuildControlElementPanel()
{
    m_panelControlElements->SetDoubleBuffered(true);
    wxWrapSizer* wrapSizer = new wxWrapSizer();
    m_panelControlElements->SetSizer(wrapSizer);

    ControlElementButton* ioButton = new ControlElementButton(
        m_panelControlElements, _("In/Out"), wxImage("..\\data\\images\\control\\io.png"), ID_IO);
    wrapSizer->Add(ioButton, 0, wxALL, 5);
    ioButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* tfButton = new ControlElementButton(
        m_panelControlElements, _("Transfer fcn"), wxImage("..\\data\\images\\control\\transferFunc.png"), ID_TF);
    wrapSizer->Add(tfButton, 0, wxALL, 5);
    tfButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* sumButton = new ControlElementButton(
        m_panelControlElements, _("Sum"), wxImage("..\\data\\images\\control\\sum.png"), ID_SUM);
    wrapSizer->Add(sumButton, 0, wxALL, 5);
    sumButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* constButton = new ControlElementButton(
        m_panelControlElements, _("Constant"), wxImage("..\\data\\images\\control\\value.png"), ID_CONST);
    wrapSizer->Add(constButton, 0, wxALL, 5);
    constButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* limButton = new ControlElementButton(
        m_panelControlElements, _("Limiter"), wxImage("..\\data\\images\\control\\limiter.png"), ID_LIMITER);
    wrapSizer->Add(limButton, 0, wxALL, 5);
    limButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* gainButton = new ControlElementButton(
        m_panelControlElements, _("Gain"), wxImage("..\\data\\images\\control\\gain.png"), ID_GAIN);
    wrapSizer->Add(gainButton, 0, wxALL, 5);
    gainButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* multButton = new ControlElementButton(
        m_panelControlElements, _("Multiplier"), wxImage("..\\data\\images\\control\\mult.png"), ID_MULT);
    wrapSizer->Add(multButton, 0, wxALL, 5);
    multButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* satButton = new ControlElementButton(
        m_panelControlElements, _("Saturation"), wxImage("..\\data\\images\\control\\sat.png"), ID_SAT);
    wrapSizer->Add(satButton, 0, wxALL, 5);
    satButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* rateLimButton = new ControlElementButton(
        m_panelControlElements, _("Rate limiter"), wxImage("..\\data\\images\\control\\rateLimiter.png"), ID_RATELIM);
    wrapSizer->Add(rateLimButton, 0, wxALL, 5);
    rateLimButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);
}

void ControlEditor::LeftClickDown(wxMouseEvent& event)
{
    AddElement(static_cast<ControlElementButtonID>(event.GetId()));
    event.Skip();
}

void ControlEditor::SetViewport()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background.
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
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

void ControlEditor::AddElement(ControlElementButtonID id)
{
    switch(id) {
        case ID_IO: {
            wxLogMessage("io");
        } break;
        case ID_TF: {
            m_mode = MODE_INSERT;
            TransferFunction* tf = new TransferFunction();
            m_elementList.push_back(tf);
        } break;
        case ID_SUM: {
            m_mode = MODE_INSERT;
            Sum* sum = new Sum();
            m_elementList.push_back(sum);
        } break;
        case ID_CONST: {
            wxLogMessage("const");
        } break;
        case ID_LIMITER: {
            wxLogMessage("limiter");
        } break;
        case ID_GAIN: {
            wxLogMessage("gain");
        } break;
        case ID_MULT: {
            wxLogMessage("mult");
        } break;
        case ID_SAT: {
            wxLogMessage("sat");
        } break;
        case ID_RATELIM: {
            wxLogMessage("rateLim");
        } break;
    }
}

void ControlEditor::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(m_glCanvas);
    m_glContext->SetCurrent(*m_glCanvas);
    SetViewport();

    // Set GLCanvas scale and translation.
    glScaled(m_camera->GetScale(), m_camera->GetScale(), 0.0);                         // Scale
    glTranslated(m_camera->GetTranslation().m_x, m_camera->GetTranslation().m_y, 0.0); // Translation

    for(auto it = m_connectionList.begin(), itEnd = m_connectionList.end(); it != itEnd; ++it) {
        ConnectionLine* line = *it;
        line->Draw(m_camera->GetTranslation(), m_camera->GetScale());
    }

    for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        Element* element = *it;
        element->Draw(m_camera->GetTranslation(), m_camera->GetScale());
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

    glFlush(); // Sends all pending information directly to the GPU.
    m_glCanvas->SwapBuffers();
    event.Skip();
}

void ControlEditor::OnDoubleClick(wxMouseEvent& event)
{
    wxPoint2DDouble clickPoint = event.GetPosition();
    bool redraw = false;

    if(m_mode == MODE_EDIT) {
        for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            Element* element = *it;
            if(element->Contains(m_camera->ScreenToWorld(clickPoint))) {
                element->ShowForm(this, element);
                CheckConnections();
                auto childList = element->GetChildList();
                for(auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; ++itC) {
                    ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
                    line->UpdatePoints();
                }
                redraw = true;
            }
        }
    }

    if(redraw) Redraw();
}

void ControlEditor::OnLeftClickDown(wxMouseEvent& event)
{
    wxPoint2DDouble clickPoint = event.GetPosition();
    bool foundElement = false;

    if(m_mode == MODE_INSERT) {
        m_mode = MODE_EDIT;
    } else {
        for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
            ControlElement* element = *it;
            bool foundNode = false;
            auto nodeList = element->GetNodeList();
            for(auto itN = nodeList.begin(), itNEnd = nodeList.end(); itN != itNEnd; ++itN) {
                Node* node = *itN;
                if(node->Contains(m_camera->ScreenToWorld(clickPoint))) {
                    m_mode = MODE_INSERT_LINE;
                    ConnectionLine* line = new ConnectionLine(node);
                    m_connectionList.push_back(line);
                    element->AddChild(line);
                    line->AddParent(element);
                    foundElement = true;
                    foundNode = true;
                }
            }

            if(!foundNode) {
                // Set movement initial position (not necessarily will be moved).
                element->StartMove(m_camera->ScreenToWorld(clickPoint));

                // Click in an element.
                if(element->Contains(m_camera->ScreenToWorld(clickPoint))) {
                    if(!foundElement) {
                        element->SetSelected();
                        foundElement = true;
                    }
                    m_mode = MODE_MOVE_ELEMENT;
                }
            }
        }
        if(m_mode != MODE_INSERT_LINE) {
            for(auto it = m_connectionList.begin(), itEnd = m_connectionList.end(); it != itEnd; ++it) {
                ConnectionLine* line = *it;
                line->StartMove(m_camera->ScreenToWorld(clickPoint));
                if(line->Contains(m_camera->ScreenToWorld(clickPoint))) {
                    line->SetSelected();
                    foundElement = true;
                    m_mode = MODE_MOVE_LINE;
                }
            }
        }
    }

    if(!foundElement) {
        m_mode = MODE_SELECTION_RECT;
        m_startSelRect = m_camera->ScreenToWorld(clickPoint);
    }

    Redraw();
    event.Skip();
}

void ControlEditor::OnLeftClickUp(wxMouseEvent& event)
{
    bool foundNode = false;
    for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; it++) {
        ControlElement* element = *it;
        if(m_mode == MODE_INSERT_LINE) {
            auto nodeList = element->GetNodeList();
            for(auto itN = nodeList.begin(), itNEnd = nodeList.end(); itN != itNEnd; ++itN) {
                Node* node = *itN;
                if(node->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                    ConnectionLine* line = *(m_connectionList.end() - 1);
                    if(line->AppendNode(node, element)) {
                        line->AddParent(element);
                        element->AddChild(line);
                        line->UpdatePoints();
                        m_mode = MODE_EDIT;
                        foundNode = true;
                    }
                }
            }
        } else if(m_mode == MODE_SELECTION_RECT) {
            if(element->Intersects(m_selectionRect)) {
                element->SetSelected();
            } else if(!event.ControlDown()) {
                element->SetSelected(false);
            }
        } else if(!event.ControlDown()) {
            if(!element->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                element->SetSelected(false);
            }
        }
    }
    for(auto it = m_connectionList.begin(), itEnd = m_connectionList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        if(m_mode == MODE_INSERT_LINE && !foundNode && it != (itEnd - 1)) {
            if(cLine->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                ConnectionLine* iLine = *(m_connectionList.end() - 1);
                if(iLine->SetParentLine(cLine)) {
                    cLine->AddChild(iLine);
                    iLine->UpdatePoints();
                    m_mode = MODE_EDIT;
                    foundNode = true;
                }
            }
        } else if(m_mode == MODE_SELECTION_RECT) {
            if(cLine->Intersects(m_selectionRect)) {
                cLine->SetSelected();
            } else if(!event.ControlDown()) {
                cLine->SetSelected(false);
            }
        } else if(!event.ControlDown()) {
            if(!cLine->Contains(m_camera->ScreenToWorld(event.GetPosition()))) {
                cLine->SetSelected(false);
            }
        }
    }

    m_selectionRect = wxRect2DDouble(0, 0, 0, 0);

    if(m_mode == MODE_INSERT_LINE && !foundNode) {
        ConnectionLine* cLine = *(m_connectionList.end() - 1);
        // Free nodes
        auto nodeList = cLine->GetNodeList();
        for(auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
            Node* node = *itN;
            node->SetConnected(false);
        }
        // Remove the associated child from parents.
        auto parentList = cLine->GetParentList();
        for(auto it = parentList.begin(), itEnd = parentList.end(); it != itEnd; ++it) {
            Element* element = *it;
            element->RemoveChild(cLine);
        }
        m_connectionList.pop_back();
        if(cLine) delete cLine;
        m_mode = MODE_EDIT;
    } else if(m_mode != MODE_INSERT) {
        m_mode = MODE_EDIT;
    }

    Redraw();
    event.Skip();
}

void ControlEditor::OnMiddleDown(wxMouseEvent& event)
{
    // Set to drag mode.
    switch(m_mode) {
        case MODE_INSERT: {
            m_mode = MODE_DRAG_INSERT;
        } break;
        case MODE_PASTE: {
            m_mode = MODE_DRAG_PASTE;
        } break;
        default: {
            m_mode = MODE_DRAG;
        } break;
    }
    m_camera->StartTranslation(m_camera->ScreenToWorld(event.GetPosition()));
}

void ControlEditor::OnMiddleUp(wxMouseEvent& event)
{
    switch(m_mode) {
        case MODE_DRAG_INSERT: {
            m_mode = MODE_INSERT;
        } break;
        case MODE_DRAG_PASTE: {
            m_mode = MODE_PASTE;
        } break;
        case MODE_INSERT:
        case MODE_PASTE: {
            // Does nothing.
        } break;
        default: {
            m_mode = MODE_EDIT;
        } break;
    }
}

void ControlEditor::OnMouseMotion(wxMouseEvent& event)
{
    wxPoint2DDouble clickPoint = event.GetPosition();
    bool redraw = false;

    switch(m_mode) {
        case MODE_INSERT: {
            Element* newElement = *(m_elementList.end() - 1); // Get the last element in the list.
            newElement->Move(m_camera->ScreenToWorld(clickPoint));
            redraw = true;
        } break;
        case MODE_INSERT_LINE: {
            ConnectionLine* line = *(m_connectionList.end() - 1);
            line->SetTemporarySecondPoint(m_camera->ScreenToWorld(clickPoint));
            line->UpdatePoints();
            redraw = true;
        } break;
        case MODE_DRAG:
        case MODE_DRAG_INSERT:
        case MODE_DRAG_PASTE: {
            m_camera->SetTranslation(clickPoint);
            redraw = true;
        } break;
        case MODE_MOVE_ELEMENT: {
            for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; it++) {
                Element* element = *it;
                if(element->IsSelected()) {
                    element->Move(m_camera->ScreenToWorld(clickPoint));
                    auto childList = element->GetChildList();
                    for(auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; itC++) {
                        ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
                        line->UpdatePoints();
                    }
                    redraw = true;
                }
            }
        } break;
        case MODE_MOVE_LINE: {
            for(auto it = m_connectionList.begin(), itEnd = m_connectionList.end(); it != itEnd; it++) {
                ConnectionLine* line = *it;
                if(line->IsSelected()) {
                    line->Move(m_camera->ScreenToWorld(clickPoint));
                    redraw = true;
                }
            }
        } break;
        case MODE_SELECTION_RECT: {
            wxPoint2DDouble currentPos = m_camera->ScreenToWorld(clickPoint);
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
        default:
            break;
    }

    if(redraw) Redraw();
    event.Skip();
}

void ControlEditor::OnScroll(wxMouseEvent& event)
{
    if(event.GetWheelRotation() > 0)
        m_camera->SetScale(event.GetPosition(), +0.05);
    else
        m_camera->SetScale(event.GetPosition(), -0.05);

    Redraw();
}
void ControlEditor::OnIdle(wxIdleEvent& event)
{
    // Solve wxGLString bug.
    if(m_firstDraw) {
        TransferFunction* tf = new TransferFunction();
        m_elementList.push_back(tf);
        Redraw();
        m_elementList.pop_back();
        delete tf;
        m_firstDraw = false;
    }
}
void ControlEditor::OnKeyDown(wxKeyEvent& event)
{
    char key = event.GetUnicodeKey();
    if(key != WXK_NONE) {
        switch(key) {
            case WXK_DELETE: // Delete selected elements.
            {
                DeleteSelectedElements();
            } break;
            case 'R': // Rotate the selected elements.
            {
                RotateSelectedElements(event.GetModifiers() != wxMOD_SHIFT);
            } break;
        }
    }
}

void ControlEditor::RotateSelectedElements(bool clockwise)
{
    for(auto it = m_elementList.begin(), itEnd = m_elementList.end(); it != itEnd; ++it) {
        Element* element = *it;
        if(element->IsSelected()) {
            element->Rotate(clockwise);
            auto childList = element->GetChildList();
            for(auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; itC++) {
                ConnectionLine* line = static_cast<ConnectionLine*>(*itC);
                line->UpdatePoints();
            }
        }
    }
    Redraw();
}

void ControlEditor::DeleteSelectedElements()
{
    for(auto it = m_elementList.begin(); it != m_elementList.end(); ++it) {
        Element* element = *it;
        if(element->IsSelected()) {
            // Remove child/parent.
            auto childList = element->GetChildList();
            for(auto itC = childList.begin(), itEnd = childList.end(); itC != itEnd; ++itC) {
                // The child is always a connection line.
                ConnectionLine* child = static_cast<ConnectionLine*>(*itC);
                // Delete the connection line.
                for(auto itCo = m_connectionList.begin(); itCo != m_connectionList.end(); ++itCo) {
                    ConnectionLine* line = *itCo;
                    if(line == child) {
                        itCo = DeleteLineFromList(itCo);
                        break;
                    }
                }
            }
            m_elementList.erase(it--);
            if(element) delete element;
        }
    }

    for(auto it = m_connectionList.begin(); it != m_connectionList.end(); ++it) {
        ConnectionLine* line = *it;
        if(line->IsSelected()) {
            it = DeleteLineFromList(it);
        }
    }
    Redraw();
}

std::vector<ConnectionLine*>::iterator ControlEditor::DeleteLineFromList(std::vector<ConnectionLine*>::iterator& it)
{
    ConnectionLine* cLine = *it;
    auto childList = cLine->GetLineChildList();
    for(auto itC = childList.begin(), itEndC = childList.end(); itC != itEndC; ++itC) {
        ConnectionLine* child = *itC;
        for(auto itL = m_connectionList.begin(); itL != m_connectionList.end(); ++itL) {
            ConnectionLine* childOnList = *itL;
            if(childOnList == child) {
                itL = DeleteLineFromList(itL);
            }
        }
    }
    // Remove
    auto parentList = cLine->GetParentList();
    for(auto itP = parentList.begin(), itEnd = parentList.end(); itP != itEnd; ++itP) {
        Element* parent = *itP;
        if(parent) parent->RemoveChild(cLine);
    }
    if(cLine->GetParentLine()) cLine->GetParentLine()->RemoveChild(cLine);
    // Free nodes
    auto nodeList = cLine->GetNodeList();
    for(auto itN = nodeList.begin(), itEndN = nodeList.end(); itN != itEndN; ++itN) {
        Node* node = *itN;
        node->SetConnected(false);
    }
    m_connectionList.erase(it--);
    if(cLine) delete cLine;
    return it;
}

void ControlEditor::CheckConnections()
{
    for(auto it = m_connectionList.begin(); it != m_connectionList.end(); ++it) {
        ConnectionLine* cLine = *it;
        if(cLine->GetType() == ConnectionLine::ELEMENT_ELEMENT) {
            if(cLine->GetParentList().size() < 2){
                it = DeleteLineFromList(it);
            }
        }
    }
}
