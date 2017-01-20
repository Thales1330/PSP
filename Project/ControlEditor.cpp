#include "ControlEditor.h"

ControlEditor::ControlEditor(wxWindow* parent) : ControlEditorBase(parent) { BuildControlElementPanel(); }
ControlEditor::~ControlEditor()
{
    // m_tfButton->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlEditor::LeftClickDown), m_tfButton, this);
}

void ControlEditor::BuildControlElementPanel()
{
    m_panelControlElements->SetDoubleBuffered(true);
    wxWrapSizer* wrapSizer = new wxWrapSizer();
    m_panelControlElements->SetSizer(wrapSizer);

    ControlElementButton* ioButton = new ControlElementButton(m_panelControlElements, _("In/Out"),
                                                              wxImage("..\\data\\images\\control\\io.png"), ID_IO);
    wrapSizer->Add(ioButton, 0, wxALL, 5);
    ioButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* tfButton = new ControlElementButton(
        m_panelControlElements, _("Transfer fcn"), wxImage("..\\data\\images\\control\\transferFunc.png"), ID_TF);
    wrapSizer->Add(tfButton, 0, wxALL, 5);
    tfButton->Bind(wxEVT_LEFT_DOWN, &ControlEditor::LeftClickDown, this);

    ControlElementButton* sumButton = new ControlElementButton(m_panelControlElements, _("Sum"),
                                                               wxImage("..\\data\\images\\control\\sum.png"), ID_SUM);
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

    ControlElementButton* satButton = new ControlElementButton(m_panelControlElements, _("Saturation"),
                                                               wxImage("..\\data\\images\\control\\sat.png"), ID_SAT);
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

void ControlEditor::AddElement(ControlElementButtonID id)
{
    switch(id) {
        case ID_IO: {
            wxLogMessage("io");
        } break;
        case ID_TF: {
            wxLogMessage("tf");
        } break;
        case ID_SUM: {
            wxLogMessage("sum");
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

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, wxImage image, wxWindowID id)
    : wxWindow(parent, id)
{
    SetBackgroundColour(*wxWHITE);
    //m_font = wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
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
