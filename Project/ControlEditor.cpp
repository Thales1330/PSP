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

    /*m_tfButton = new wxButton();
    m_tfButton->SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
    m_tfButton->Create(m_panelControlElements, wxID_ANY, _("Transfer function"), wxDefaultPosition, wxDefaultSize,
    wxBORDER_NONE);
    m_tfButton->SetTransparent(wxIMAGE_ALPHA_TRANSPARENT);
    //m_tfButton = new wxButton(m_panelControlElements, wxID_ANY, _("Transfer function"), wxDefaultPosition,
    wxDefaultSize, wxBORDER_NONE);
    m_tfButton->SetBitmap(wxImage("..\\data\\images\\control\\transferFunc.png"), wxTOP);
    // m_tfButton->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    // m_tfButton->SetToolTip(_("Transfer function"));
    m_tfButton->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ControlEditor::LeftClickDown), NULL, this);
    wrapSizer->Add(m_tfButton, 0, wxALL, 3)*/
    ControlElementButton* tfButton = new ControlElementButton(
        m_panelControlElements, _("Transfer function"), wxImage("..\\data\\images\\control\\transferFunc.png"));
    wrapSizer->Add(tfButton, 0, wxALL, 5);

    wxBitmapButton* tfButton2 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton2->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton2->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton2, 0, wxALL, 3);

    wxBitmapButton* tfButton3 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton3->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton3->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton3, 0, wxALL, 3);

    wxBitmapButton* tfButton4 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton4->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton4->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton4, 0, wxALL, 3);

    wxBitmapButton* tfButton5 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton5->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton5->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton5, 0, wxALL, 3);

    wxBitmapButton* tfButton6 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton6->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton6->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton6, 0, wxALL, 3);

    wxBitmapButton* tfButton7 =
        new wxBitmapButton(m_panelControlElements, wxID_ANY, wxImage("..\\data\\images\\control\\transferFunc.png"),
            wxDefaultPosition, wxDLG_UNIT(m_panelControlElements, wxSize(-1, -1)), wxBU_AUTODRAW | wxBORDER_NONE);
    tfButton7->SetBackgroundColour(wxColour(wxT("rgb(255,255,255)")));
    tfButton7->SetToolTip(_("Transfer function"));

    wrapSizer->Add(tfButton7, 0, wxALL, 3);

    // m_auimgr->Update();
    // Layout();
}
void ControlEditor::LeftClickDown(wxMouseEvent& event)
{

    wxBitmapButton* button = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
    if(button) {
        button->SetOwnBackgroundColour(wxColour(wxT("rgb(0,0,255)")));
    }
    event.Skip();
}

ControlElementButton::ControlElementButton(wxWindow* parent, wxString label, wxImage image)
    : wxWindow(parent, wxID_ANY)
{
    m_label = label;
    m_image = image;
    SetMinSize(wxSize(32,32));
    Connect(wxEVT_PAINT, wxPaintEventHandler(ControlElementButton::OnPaint), NULL, this);
}

ControlElementButton::~ControlElementButton() {}

void ControlElementButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    //dc.SetBrush(*wxGREY_BRUSH);
    //dc.DrawRectangle( 0, 0, 32, 32 );
    dc.DrawBitmap(m_image, wxPoint(0,0));
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if(gc) {
        gc->DrawBitmap(gc->CreateBitmapFromImage(m_image), 0, 0, 32, 32);
        gc->SetBrush(wxBrush(wxColour(0,125,255,100)));
        gc->DrawRectangle(0,0,32,32);
        delete gc;
    }
}
