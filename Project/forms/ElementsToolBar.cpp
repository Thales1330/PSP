#include "ElementsToolBar.h"
#include "../MainFrame.h"
#include "../utils/Path.h"
#include <wx/sizer.h>
#include <wx/bitmap.h>

ElementsToolBar::ElementsToolBar(MainFrame* parent)
	: wxFrame(parent, wxID_ANY, _("Elements"), wxDefaultPosition, wxDefaultSize,
		wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW | wxFRAME_NO_TASKBAR),
	m_mainFrame(parent)
{
	InitToolBar();

	Bind(wxEVT_CLOSE_WINDOW, &ElementsToolBar::OnClose, this);
}

ElementsToolBar::~ElementsToolBar()
{
}

void ElementsToolBar::InitToolBar()
{
	m_toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxTB_VERTICAL | wxTB_FLAT | wxTB_NODIVIDER);
	m_toolBar->SetToolBitmapSize(wxSize(24, 24));

	wxString imgPath = Paths::GetDataPath() + "/images/elements/";

	m_toolBar->AddTool(ID_ADDMENU_BUS, _("Bus"), wxBitmap(imgPath + "bus.png", wxBITMAP_TYPE_PNG), _("Adds a bus at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_LINE, _("Line"), wxBitmap(imgPath + "line.png", wxBITMAP_TYPE_PNG), _("Adds a power line at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_TRANSFORMER, _("Transformer"), wxBitmap(imgPath + "transformer.png", wxBITMAP_TYPE_PNG), _("Adds a transformer at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_GENERATOR, _("Generator"), wxBitmap(imgPath + "generator.png", wxBITMAP_TYPE_PNG), _("Adds a generator at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_INDMOTOR, _("Induction motor"), wxBitmap(imgPath + "indmotor.png", wxBITMAP_TYPE_PNG), _("Adds an induction motor at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_SYNCCOMP, _("Synchronous compensator"), wxBitmap(imgPath + "synccomp.png", wxBITMAP_TYPE_PNG), _("Adds a synchronous compensator at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_LOAD, _("Load"), wxBitmap(imgPath + "load.png", wxBITMAP_TYPE_PNG), _("Adds a load at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_CAPACITOR, _("Capacitor"), wxBitmap(imgPath + "capacitor.png", wxBITMAP_TYPE_PNG), _("Adds a shunt capacitor at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_INDUCTOR, _("Inductor"), wxBitmap(imgPath + "inductor.png", wxBITMAP_TYPE_PNG), _("Adds a shunt inductor at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_HARMCURRENT, _("Harmonic current"), wxBitmap(imgPath + "harmcurrent.png", wxBITMAP_TYPE_PNG), _("Adds a harmonic current source at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_EMTELEMENT, _("EMT Element"), wxBitmap(imgPath + "emtelement.png", wxBITMAP_TYPE_PNG), _("Adds an electromagnetic transient element that connects with ATP"));
	m_toolBar->AddTool(ID_ADDMENU_TEXT, _("Label"), wxBitmap(imgPath + "text.png", wxBITMAP_TYPE_PNG), _("Adds a linked element label"));

	m_toolBar->Realize();

	m_toolBar->Bind(wxEVT_TOOL, &ElementsToolBar::OnToolClicked, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_toolBar, 1, wxEXPAND);
	SetSizerAndFit(sizer);
}

void ElementsToolBar::EnableTools(bool enable)
{
	if (!m_toolBar) return;
	int ids[] = {
		ID_ADDMENU_BUS, ID_ADDMENU_LINE, ID_ADDMENU_TRANSFORMER,
		ID_ADDMENU_GENERATOR, ID_ADDMENU_INDMOTOR, ID_ADDMENU_SYNCCOMP,
		ID_ADDMENU_LOAD, ID_ADDMENU_CAPACITOR, ID_ADDMENU_INDUCTOR,
		ID_ADDMENU_HARMCURRENT, ID_ADDMENU_EMTELEMENT, ID_ADDMENU_TEXT
	};
	for (int id : ids) {
		m_toolBar->EnableTool(id, enable);
	}
}

void ElementsToolBar::OnToolClicked(wxCommandEvent& event)
{
	if (m_mainFrame) {
		m_mainFrame->OnAddElementsClick(event);
	}
}

void ElementsToolBar::OnClose(wxCloseEvent& event)
{
	Hide();
	if (m_mainFrame) {
		m_mainFrame->OnElementsToolBarClosed();
	}
}
