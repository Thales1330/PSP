#include "ElementsToolBar.h"
#include "../MainFrame.h"
#include "../utils/Path.h"
#include <wx/sizer.h>
#include <wx/bitmap.h>

ElementsToolBar::ElementsToolBar(MainFrame* parent, bool vertical)
	: wxFrame(parent, wxID_ANY, _("Power Elements"), wxDefaultPosition, wxDefaultSize,
		wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_NO_TASKBAR),
	m_mainFrame(parent)
{
	InitToolBar(vertical);
	Bind(wxEVT_TOOL, &ElementsToolBar::OnToolClicked, this);
	Bind(wxEVT_CLOSE_WINDOW, &ElementsToolBar::OnClose, this);
}

ElementsToolBar::~ElementsToolBar()
{
}

void ElementsToolBar::InitToolBar(bool vertical)
{
	m_toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		(vertical ? wxTB_VERTICAL : wxTB_HORIZONTAL) | wxTB_FLAT | wxTB_NODIVIDER);
	m_toolBar->SetToolBitmapSize(wxSize(32, 32));

	wxString imgPath = Paths::GetDataPath() + "/images/elements/";

	m_toolBar->AddTool(ID_ADDMENU_BUS, _("Bus"), LoadToolBitmap(imgPath + "bus.png"), _("Add a bus at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_LINE, _("Line"), LoadToolBitmap(imgPath + "line.png"), _("Add a power line at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_TRANSFORMER, _("Transformer"), LoadToolBitmap(imgPath + "transformer.png"), _("Add a transformer at the circuit"));

	m_toolBar->AddTool(ID_ADDMENU_GENERATOR, _("Generator"), LoadToolBitmap(imgPath + "generator.png"), _("Add a generator at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_INDMOTOR, _("Induction motor"), LoadToolBitmap(imgPath + "indmotor.png"), _("Add an induction motor at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_SYNCCOMP, _("Synchronous compensator"), LoadToolBitmap(imgPath + "synccomp.png"), _("Add a synchronous compensator at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_LOAD, _("Load"), LoadToolBitmap(imgPath + "load.png"), _("Add a load at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_CAPACITOR, _("Capacitor"), LoadToolBitmap(imgPath + "capacitor.png"), _("Add a shunt capacitor at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_INDUCTOR, _("Inductor"), LoadToolBitmap(imgPath + "inductor.png"), _("Add a shunt inductor at the circuit"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_HARMCURRENT, _("Harmonic current"), LoadToolBitmap(imgPath + "harmcurrent.png"), _("Add a harmonic current source at the circuit"));
	m_toolBar->AddTool(ID_ADDMENU_EMTELEMENT, _("EMT Element"), LoadToolBitmap(imgPath + "emtelement.png"), _("Add an electromagnetic transient element"));
	m_toolBar->AddSeparator();

	m_toolBar->AddTool(ID_ADDMENU_TEXT, _("Label"), LoadToolBitmap(imgPath + "text.png"), _("Add a linked element label"));

	m_toolBar->Realize();

	//m_toolBar->Bind(wxEVT_TOOL, &ElementsToolBar::OnToolClicked, this);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_toolBar, 1, wxEXPAND);
	SetSizerAndFit(sizer);
}

wxBitmap ElementsToolBar::LoadToolBitmap(const wxString& path)
{
	wxImage image(path, wxBITMAP_TYPE_PNG);

	if (!image.IsOk()) {
		return wxNullBitmap;
	}

	wxSize size = FromDIP(wxSize(32, 32));

	image.Rescale(size.x, size.y, wxIMAGE_QUALITY_HIGH);

	return wxBitmap(image);
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
	if (event.CanVeto())
		event.Veto();

	Hide();
	if (m_mainFrame) {
		m_mainFrame->OnElementsToolBarClosed();
	}
}
