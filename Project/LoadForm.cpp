#include "LoadForm.h"
#include "Load.h"

LoadForm::LoadForm(wxWindow* parent, Load* load)
    : LoadFormBase(parent)
{
    LoadElectricalData data = load->GetElectricalData();
    
    m_textCtrlName->SetValue(data.name);
    
    m_textCtrlActivePower->SetValue(Load::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case UNIT_PU: {
            m_choiceActivePower->SetSelection(0);
        }
        case UNIT_W: {
            m_choiceActivePower->SetSelection(1);
        }
        case UNIT_kW: {
            m_choiceActivePower->SetSelection(2);
        }
        case UNIT_MW: {
            m_choiceActivePower->SetSelection(3);
        }
    }
    
    m_parent = parent;
    m_load = load;
}

LoadForm::~LoadForm()
{
}

void LoadForm::OnCancelButtonClick(wxCommandEvent& event)
{
}
void LoadForm::OnOnButtonClick(wxCommandEvent& event)
{
}
void LoadForm::OnStabilityButtonClick(wxCommandEvent& event)
{
}
