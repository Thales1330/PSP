#include "ControlEditor.h"

ControlEditor::ControlEditor(wxWindow* parent)
    : ControlEditorBase(parent)
{
    BuildControlElementPanel();
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
        m_panelControlElements, _("Input/output"), wxImage("..\\data\\images\\control\\io.png"));
    wrapSizer->Add(ioButton, 0, wxALL, 5);

    ControlElementButton* tfButton = new ControlElementButton(
        m_panelControlElements, _("Transfer function"), wxImage("..\\data\\images\\control\\transferFunc.png"));
    wrapSizer->Add(tfButton, 0, wxALL, 5);

    ControlElementButton* sumButton =
        new ControlElementButton(m_panelControlElements, _("Sum"), wxImage("..\\data\\images\\control\\sum.png"));
    wrapSizer->Add(sumButton, 0, wxALL, 5);

    ControlElementButton* valueButton =
        new ControlElementButton(m_panelControlElements, _("Value"), wxImage("..\\data\\images\\control\\value.png"));
    wrapSizer->Add(valueButton, 0, wxALL, 5);

    ControlElementButton* limButton = new ControlElementButton(
        m_panelControlElements, _("Limiter"), wxImage("..\\data\\images\\control\\limiter.png"));
    wrapSizer->Add(limButton, 0, wxALL, 5);

    ControlElementButton* gainButton =
        new ControlElementButton(m_panelControlElements, _("Gain"), wxImage("..\\data\\images\\control\\gain.png"));
    wrapSizer->Add(gainButton, 0, wxALL, 5);

    ControlElementButton* multButton = new ControlElementButton(
        m_panelControlElements, _("Multiplier"), wxImage("..\\data\\images\\control\\mult.png"));
    wrapSizer->Add(multButton, 0, wxALL, 5);

    ControlElementButton* satButton = new ControlElementButton(
        m_panelControlElements, _("Saturation"), wxImage("..\\data\\images\\control\\sat.png"));
    wrapSizer->Add(satButton, 0, wxALL, 5);

    ControlElementButton* rateLimButton = new ControlElementButton(
        m_panelControlElements, _("Rate limiter"), wxImage("..\\data\\images\\control\\rateLimiter.png"));
    wrapSizer->Add(rateLimButton, 0, wxALL, 5);
}

void ControlEditor::LeftClickDown(wxMouseEvent& event)
{

    /*wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    if(button) {
        button->SetOwnBackgroundColour(wxColour(wxT("rgb(0,0,255)")));
    }*/
    event.Skip();
}

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, wxImage image)
    : wxWindow(parent, wxID_ANY)
{
    m_font = wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
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
    SetMinSize(m_buttonSize);

    // Conncet events.
    Connect(wxEVT_PAINT, wxPaintEventHandler(ControlElementButton::OnPaint), NULL, this);
    Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(ControlElementButton::OnMouseEnter), NULL, this);
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(ControlElementButton::OnMouseLeave), NULL, this);
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlElementButton::OnLeftClickDown), NULL, this);
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ControlElementButton::OnLeftClickUp), NULL, this);
}

ControlElementButton::~ControlElementButton() {}

void ControlElementButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if(gc) {
        gc->SetPen(*wxWHITE_PEN);
        gc->SetBrush(*wxWHITE_BRUSH);
        gc->DrawRectangle(0, 0, m_buttonSize.GetWidth(), m_buttonSize.GetHeight());
        gc->DrawBitmap(gc->CreateBitmapFromImage(m_image), m_imagePosition.x, m_imagePosition.y, m_imageSize.GetWidth(),
            m_imageSize.GetHeight());
        if(m_mouseAbove) {
            if(m_selected) {
                gc->SetPen(wxPen(wxColour(0, 125, 255, 255), m_borderSize));
                gc->SetBrush(wxBrush(wxColour(0, 125, 255, 100)));
            } else {
                gc->SetPen(*wxTRANSPARENT_PEN);
                gc->SetBrush(wxBrush(wxColour(0, 125, 255, 70)));
            }
            gc->DrawRectangle(0, 0, m_buttonSize.GetWidth(), m_buttonSize.GetHeight());
        }
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
